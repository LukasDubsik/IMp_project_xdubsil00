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
#include "filesystem/commands.h"
#include "imp.h"
#include "periferies/led.h"
#include "periferies/uart.h"
#include "periferies/little_fs.h"

//System headers
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_log.h"

//Standart header
#include <stdbool.h>


void app_main(void)
{
    /* 0) Declare and define the values that will present thorought */
    char current_dir[MAX_DIR_EXPANSION] = {0};  //The directory of the file system we are creating and in which we currently are in
    char rx_buffer[RX_BUFFER] = {0};

    /* 1) Run configuration on peripherals that will be used */
    setup_hardware();
    mount_little_fs();

    //Start the infinite iteration
    while(1){
        //Start the singular session iteration
        while(1){
            //At the start of each cycle blink long led flashe to signal start of new cycle - in case of errors 
            blink_led(CORRECT_LED_TIME);

            //Reset the values
            int passed = set_new_directory_path(current_dir, "/"); //Always start in the same directory - the root
            //While genrally it is okay if this fails - here it would brek the whole system, so exit
            if (!passed){
                break;
            }


            /* 2) Wait for the user to select the mode of the program (1,2,3,4) */
            char mode = '\0';
            bool restart = scan_for_selection("1234", &mode);
            //By breaking out here we still remain in the loop that we will never escape
            if (restart){
                break;
            }


            /* 3) Setup the file system based on the selected mode */
            if (mode == '1'){
                passed = set_new_directory_path(current_dir, LITTLE_FS_BASE_PATH); //Always start in the same directory - the root
                //While genrally it is okay if this fails - here it would brek the whole system, so exit
                if (!passed){
                    break;
            }
            }
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
            //The first message is the prompt, which will be printed on the PI and boot the file system
            uart_send_prompt(current_dir);

            /* 6.2) Wait for response - signaling all is running correctly on its end */
            //Set a waiting time
            int64_t limit = esp_timer_get_time() + WAIT_FOR_PI;
            //Iterate until the time limit runs out
            char ini_contact = uart_read(rx_buffer, true, limit);
            //Check if the message was received
            if (ini_contact != UART_READ_PASSED){
                //Just try to send message once again - not guaranteed to pass but just try
                uart_send_data("Communication from PI not working!");
                //Flash based on error type
                if (ini_contact ==  UART_READ_BUFFER_OVERFLOW){
                    ESP_LOGI(TAG, "OVERFLOW!");
                    blink_error(LED_BUFFER_READ_OVERFLOW);
                }
                else if (ini_contact == UART_READ_TIMEOUT){
                    ESP_LOGI(TAG, "TIMEOUT!");
                    blink_error(LED_READ_TIMEOUT);
                }
                //break the loop and start again
                break;
            }
            //The char read
            ESP_LOGI(TAG, "Pressed the key: %c!", rx_buffer[0]);
            //Then check what the message was - expecting just !
            if (rx_buffer[0] != '!'){
                ESP_LOGI(TAG, "INCORRECT RETURN!");
                blink_error(LED_UNEXPECTED_MESSAGE);
                break;
            }
            //Now that this is setup we can finally start fully coimmunicating
        

            /* 7) Eternal processing of user inputs */
            while(1){
                /* 7.1) Wait for the input from the PI4 (user command) */
                uart_read(rx_buffer, false, 0);

                blink_error(LED_UNEXPECTED_MESSAGE);

                /* 7.2) Process the given command */
                select_command(rx_buffer, current_dir);

                /* 7.3) Retrun back to the 7.1 */
                blink_error(LED_UNEXPECTED_MESSAGE);
            }


            /* 8) Once here the system has ended its run - restart to the start */
            //TODO


            //A temporary break to restart the whole system before correct code was set in
            break;
        }
    }
}