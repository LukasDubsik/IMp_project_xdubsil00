//User defined Includes
#include "matrix.h"
#include"led.h"
#include "../imp.h"

//Standard library
#include <string.h>

//System Includes
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "esp_rom_sys.h"


/* LOCAL VARIABLES */
//Maps where the pins are for rows
static gpio_num_t ROW_PINS[2] = { GPIO_NUM_25, GPIO_NUM_26 };
//Maps where the pins are for the columns - Due to viable pins and hraware limitation only first two rows will be assigned
static gpio_num_t COLUMN_PINS[4] = { GPIO_NUM_13, GPIO_NUM_12, GPIO_NUM_14, GPIO_NUM_27 };

//Current state of the button - Zero meaning up, 1 down
static uint8_t  active_state[4][4] = {0};
//Until what time to wait with output - stops bouncing output
static int64_t  wait_until[4][4] = {{0}};

//Store the outline of the Keyboard
static const char KEYMAP[4][4] = {
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' }
};


void configure_keyboard(void)
{
    //Set the rows - set as outputs
    gpio_config_t rows = {
        .pin_bit_mask = 0, //Preset to 0 so uit doesn't apply to any pin
        .mode = GPIO_MODE_OUTPUT, //Mode is set to output - generate signals
        .pull_up_en = GPIO_PULLUP_DISABLE, //Set weak resistor to 3.3V - disable
        .pull_down_en = GPIO_PULLDOWN_DISABLE, //Set weak resistor to GND - disable
        .intr_type = GPIO_INTR_DISABLE //If to generate interrupts - disable
    };

    //Set the pin mask to one for the specified pins
    for (int i = 0; i < 2; ++i) {
        rows.pin_bit_mask |= (1ULL << ROW_PINS[i]);
    }
    //Configure the selected pins based on my mask settings
    ESP_ERROR_CHECK(gpio_config(&rows));
    //Set their outputs to HIGH on all
    for (int j = 0; j < 2; ++j) {
        gpio_set_level(ROW_PINS[j], HIGH);
    }

    //Set the columns - set as inputs
    gpio_config_t columns = {
        .pin_bit_mask = 0,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    //Set the pin mask to one for the specified pins
    for (int j = 0; j < 4; ++j) {
        columns.pin_bit_mask |= (1ULL << COLUMN_PINS[j]);
    }
    //Configure the selected pins based on my mask settings
    ESP_ERROR_CHECK(gpio_config(&columns));
    //No base value of GND/HIGH for input
}

char scan_keyboard(void)
{
    //Iterate through all rows, drive low for a second and scan the columns
    for (int row = 0; row < 2; ++row) {
        //Set to ground
        gpio_set_level(ROW_PINS[row], LOW);
        //Pause the current task for some time - no interrupt
        esp_rom_delay_us(3);

        //Now scan for each column - which is held as input
        for (int column = 0; column < 4; ++column) {
            //Get the set level of that pin
            int level = gpio_get_level(COLUMN_PINS[column]);
            //If it is pressed - based on the matrix keypad structure
            bool pressed_now = (level == LOW) ? true : false;

            //get the actual time
            //This wholesystem is done because of key bouncing present on mechanical keyboards to not read the same input multiple times
            int64_t now = esp_timer_get_time();
            //If the state differs, something must have happened
            if (pressed_now != (bool)active_state[row][column]) {
                //If the bouncing window ended we can change our output
                if (now >= wait_until[row][column]) {
                    //Change the registered value and the bounce time window
                    active_state[row][column] = pressed_now;
                    wait_until[row][column] = now + DEBOUNCE;
                    //Retrun the pressed key
                    if (pressed_now) {
                        return KEYMAP[row][column];
                    }
                } else {
                    //Still within the bounce, bouncing detected, push the timer
                    wait_until[row][column] = now + DEBOUNCE;
                }
            } else {
                //No change, keep the threshold or reset if beyond the time window
                if (now > wait_until[row][column]) {
                    wait_until[row][column] = now;
                }
            }
        }

        //Restore the given row to high to not affect the next read
        gpio_set_level(ROW_PINS[row], HIGH);
        //Another quick wait to make the changes take effect
        esp_rom_delay_us(3);
    }

    //When nothing was detected
    return '\0';
}

char wait_for_press(char* valid){
    //Iterate until a valid output is given - or restart is triggered
    while(1){
        char pressed = scan_keyboard();
        //Check if the pressed key is valid output - if so, return it
        if (pressed != '\0' && strchr(valid, pressed)){
            return pressed;
        }
        //Check if the pressed key is restart - if so, also exit
        else if (pressed == RESTART_KEY){
            return pressed;
        }
        //Otherwise inform user that wrong key was selected
        else if (pressed != '\0'){
            blink_error(INCORRECT_KEY_TYPED);
        }
        //Wait some time before the next scan
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}