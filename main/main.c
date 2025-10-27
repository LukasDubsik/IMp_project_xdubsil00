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
#include "imp.h"
#include "periferies/matrix.h"
#include "periferies/led.h"

//System headers
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"


static const char *TAG = "example";

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