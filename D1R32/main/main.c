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
#include "filesystem/filesystem.h"
#include "imp.h"
#include "periferies/led.h"
#include "periferies/uart.h"
#include "periferies/little_fs.h"

//System headers
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "tasks/tasks.h"

//Standart header
#include <stdbool.h>

// Global variables
volatile bool restart_pressed = false;
bool fs_mounted = false;
bool sd_mounted = false;
sdmmc_card_t *sd_card = NULL;

void app_main(void)
{
    /* 0) Declare and define the values that will be present thorough */
    //The directory of the file system we are creating and in which we currently are in
    char current_dir[MAX_DIR_EXPANSION] = {0};
    char rx_buffer[RX_BUFFER] = {0};

    /* 1) Run configuration on peripherals that will be used */
    setup_hardware();
    mount_little_fs();

    /* 1.1) Start tasks for watching other inputs */
    // For scanning restart on keyboard
    xTaskCreate(scan_restart, "res_watch", 2048, NULL, 1, NULL);

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
                //Always start in the same directory - the root
                passed = set_new_directory_path(current_dir, LITTLE_FS_BASE_PATH);
                //While generally it is okay if this fails - here it would break the whole system, so exit
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


            /* 5) operate based on the selected type */
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
            ESP_LOGI(TAG, "Returned from Pi: %s!", rx_buffer);
            //Then check what the message was - expecting START_CMD
            if (strcmp(rx_buffer, START_CMD) != 0){
                ESP_LOGI(TAG, "START COMMAND NOT RECEIVED!");
                blink_error(LED_UNEXPECTED_MESSAGE);
                break;
            }
            //Now that this is setup we can finally start fully communicating
        

            /* 7) Eternal processing of user inputs */
            while(1){
                /* 7.1) Reset the values */
                limit = esp_timer_get_time() + WAIT_FOR_PI;

                /* 7.2) Wait for the input from the PI4 (user command) */
                char res = uart_read(rx_buffer, true, limit);

                /* 7.3) Check restart button wasn't pressed */
                if (restart_pressed) {
                    // Restart the restart
                    restart_pressed = false;
                    // If it was pressed, inform debug
                    ESP_LOGI(TAG, "RESTART!");
                    // inform Pi4 so it can shut down
                    uart_send_data(RST_CMD);
                    // blink the restart sequence
                    blink_error(LED_RESTART_PRESSED);
                    // and exit the loop
                    break;
                }

                /* 7.4) Check that there wasn't buffer overflow */
                if (res == UART_READ_BUFFER_OVERFLOW) {
                    ESP_LOGI(TAG, "OVERFLOW!");
                    blink_error(LED_BUFFER_READ_OVERFLOW);
                    // Inform the Pi
                    uart_send_data(OVERFLOW_CMD);
                    break;
                }

                /* 7.5) Process the given command */
                if (res == UART_READ_PASSED) {
                    select_command(rx_buffer, current_dir);
                }

                /* 7.6) Return back to the 7.1 */
            }


            /* 8) Once here the system has ended its run - restart to the start */
            //TODO


            //A temporary break to restart the whole system before correct code was set in
            break;
        }
    }
}