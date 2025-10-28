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
#include "periferies/led.h"
#include "periferies/uart.h"

//System headers
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"


void app_main(void)
{
    /* 1) Run configuration on peripherals that will be used */
    setup_hardware();

    //Signal that all has been setup correctly by blinking once - for 1s
    blink_led(CORRECT_LED_TIME);

    //Start the infinite iteration
    while(1){
        //Start the singular session iteration
        while(1){
            /* 2) Wait for the user to select the mode of the program (1,2,3,4) */
            char mode = '\0';
            bool restart = scan_for_selection("1234", &mode);
            //By breaking out here we still remain in the loop that we will never escape
            if (restart){
                break;
            }


            /* 3) Setup the file system based on the selected mode */
            //TODO


            /* 4) Select the type of the operation (A, B) */
            char type = '\0';
            restart = scan_for_selection("AB", &type);
            //By breaking out here we still remain in the loop that we will never escape
            if (restart){
                break;
            }


            /* 5) operate based on teh selected type */
            //TODO


            /* 6.1) Send the message to the PI4 via UART */
            //This message doesn't have to have any meaning (no encryption excetra) 
            //We are just sending it to see if the PI is up and running
            uart_send_data("?");

            /* 6.2) Wait for response - signaling all is running correctly on its end */
            //Set a waiting time
            int64_t limit = esp_timer_get_time() + ;
            //Iterate until the time limit runs out
            while(1){

            }


            //A temporary break to restart the whole system before correct code was set in
            break;
        }
    }
}