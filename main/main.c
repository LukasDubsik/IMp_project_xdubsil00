/**
 * @file main.c
 * @brief Main part of teh application - includes the function that will be run when the program starts.
 * @author Lukas Dubsik (xdubsi00)
 * @date 26.10.2025
 * @version 0.01
 *
 * Detailed description:
 *  Starts the program etc.
 */

 //User defined headers
#include <imp.h>

//System headers
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "hal/gpio_types.h"
#include "sdkconfig.h"
#include "esp_rom_sys.h"
#include "soc/gpio_num.h"


static const char *TAG = "example";

#define BLINK_GPIO CONFIG_BLINK_GPIO
#define DEBOUNCE_US 20000  // 20 ms

static uint8_t s_led_state = 0;

/* GLOBAL VARIABLES */
//Maps where the pins are for rows
static gpio_num_t ROW_PINS[4] = { GPIO_NUM_13, GPIO_NUM_12, GPIO_NUM_14, GPIO_NUM_27 };
//Maps where the pins are for the columns - Due to viable pins and hraware limitation only first two rows will be assigned
static gpio_num_t COLUMN_PINS[4] = { GPIO_NUM_25, GPIO_NUM_26 };

//
static uint8_t  stable_state[4][4] = {0};      // current accepted (debounced) state: 0=up, 1=down
//
static int64_t  stable_until_us[4][4] = {{0}};   // per-key "do-not-change-before" timestamp

//Store the outline of the Keyboard
static const char KEYMAP[4][4] = {
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' }
};

static void configure_keyboard(void)
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
    for (int i = 0; i < 4; ++i) {
        rows.pin_bit_mask |= (1ULL << ROW_PINS[i]);
    }
    //Configure the selected pins based on my mask settings
    ESP_ERROR_CHECK(gpio_config(&rows));
    //Set their outputs to HIGH on all
    for (int j = 0; j < 4; ++j) {
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

static char scan_keyboard(void)
{
    //Iterate through all rows, drive low for a second and scan the columns
    for (int row = 0; row < 4; ++row) {
        //Set to ground
        gpio_set_level(ROW_PINS[row], LOW);
        //Pause the current task for some time - no interrupt
        esp_rom_delay_us(3);

        //Now scan for each column - which is held as input
        for (int column = 0; column < 2; ++column) {
            //Get the set level of that pin
            int level = gpio_get_level(COLUMN_PINS[column]);
            //If it is pressed - based on the matrix keypad structure
            bool pressed_now = (level == LOW) ? true : false;

            //get the actual time
            //This wholesystem is done because of key bouncing present on mechanical keyboards to not read the same input multiple times
            int64_t now = esp_timer_get_time();
            //If the state differs, something must have happened
            if (pressed_now != (bool)stable_state[row][column]) {
                //If the bouncing window ended we can change our output
                if (now >= stable_until_us[row][column]) {
                    //Change the registered value and the bounce time window
                    stable_state[row][column] = pressed_now;
                    stable_until_us[row][column] = now + DEBOUNCE_US;
                    //Retrun the pressed key
                    if (pressed_now) {
                        return KEYMAP[row][column];
                    }
                } else {
                    //Still within the bounce, bouncing detected, push the timer
                    stable_until_us[row][column] = now + DEBOUNCE_US;
                }
            } else {
                //No change, keep the threshold or reset if beyond the time window
                if (now > stable_until_us[row][column]) {
                    stable_until_us[row][column] = now;
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

static void blink_led(bool state)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(GPIO_NUM_2, state);
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(GPIO_NUM_2);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
}

void app_main(void)
{
    /* Run configuration on peripherals that will be used */
    //Configure the led - Used for informing user for small things
    configure_led();
    //Configure the keyboard - Used to select which mode to run the system in
    configure_keyboard();

    //Store the result from teh keyboard
    char pressed = '\0';

    while (1) {
        ESP_LOGI(TAG, "waiting for Input from the keyboard!");
        //Get the value from the keyboard
        while(1){
            pressed = scan_keyboard();
            //break the loop if some real input has been given
            if (pressed != '\0'){
                printf("the key that was pressed: %c\n", pressed);
                break;
            }
            //Wait some time before the next scan
            vTaskDelay(pdMS_TO_TICKS(5));
        }
        //1 turns the LED on
        if (pressed == '1'){
            blink_led(true);
        }
        //2 turns the LED off
        if (pressed == '2'){
            blink_led(false);
        }
        //vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}