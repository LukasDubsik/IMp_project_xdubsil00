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


void app_main(void)
{
    /* 1) Run configuration on peripherals that will be used */
    //Configure the led - Used for informing user for small things
    configure_led();
    //Configure the keyboard - Used to select which mode to run the system in
    configure_keyboard();

    //Signal that all has been setup correctly by blinking once - for 1s
    blink_led(CORRECT_LED_TIME);

    //Start the infinite iteration
    while(1){
        //Start the singular session iteration
        while(1){
            /* 2) Wait for the user to select the mode of the program (1,2,3,4) */
            char *valid = "1234";
            char key = wait_for_press(valid);
            ESP_LOGI(TAG, "Pressed the key: %c!", key);
            //If we have gotten the RESTART back
            if (key == RESTART_KEY){
                blink_error(3);
                //By breaking out here we still remain in the loop that we will never escape
                break;
            }
            //Otherwise blink the LED to signify the input has been accepted
            blink_led(CORRECT_LED_TIME);

            //A temporary break to restart the whole system before correct code was set in
            break;
        }
    }
}