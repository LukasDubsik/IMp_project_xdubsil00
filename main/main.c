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
            char *mode = NULL;
            bool restart = scan_for_selection("1234", mode);
            //By breaking out here we still remain in the loop that we will never escape
            if (restart){
                break;
            }


            /* 3) Select the type of the operation (A, B) */
            char *type = NULL;
            restart = scan_for_selection("AB", type);
            //By breaking out here we still remain in the loop that we will never escape
            if (restart){
                break;
            }

            //A temporary break to restart the whole system before correct code was set in
            break;
        }
    }
}