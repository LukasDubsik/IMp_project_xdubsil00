//User defined Includes
#include "benchmark/benchmark.h"
#include "imp.h"
#include "periferies/little_fs.h"
#include "periferies/matrix.h"
#include "periferies/led.h"
#include "periferies/sdcard.h"
#include "periferies/uart.h"

//Standart includes
#include <stdio.h>

//System Includes
#include "esp_log.h"
#include "esp_task_wdt.h"

void setup_hardware(void){
    //Configure the led - Used for informing user for small things
    configure_led();
    //Configure the keyboard - Used to select which mode to run the system in
    configure_keyboard();
    //Configure the UART communication for sending/receiving with the PI4
    configure_uart();
    // Turn off watchdog logging
    esp_log_level_set("task_wdt", ESP_LOG_NONE);
}

bool scan_for_selection(const char *valid, char *returned){
    *returned = wait_for_press(valid);
    //Logging to test if all is received correctly - for debugging here
    ESP_LOGI(TAG, "Pressed the key: %c!", *returned);
    //If we have gotten the RESTART back
    if (*returned == RESTART_KEY){
        //Meaning that restart was triggered
        blink_error(LED_RESTART_PRESSED);
        //Return false to inform about restart
        return true;
    }
    //Otherwise blink the LED to signify the input has been accepted
    blink_led(CORRECT_LED_TIME);
    //Return given and it wasn't a restart
    return false;
}

bool set_new_directory_path(char *dir_path, const char *new_path){
    //Temporary safe the previous content
    char *temp = malloc(MAX_DIR_EXPANSION);
    snprintf(temp, MAX_DIR_EXPANSION, "%s", dir_path);
    //"Print" the new path into the old one
    int res = snprintf(dir_path, MAX_DIR_EXPANSION, "%s", new_path);
    //Check that the current operation occured - overflow or no write
    if (res < 0 || (size_t)res >= MAX_DIR_EXPANSION) {
        //Send information to the user that overflow occured and no additional cd can occur
        uart_send_data("Overflow in directory length, returning to the previous value!");
        //Return to the original value
        snprintf(dir_path, MAX_DIR_EXPANSION, "%s", temp);
        //free resources and exit
        free(temp);
        return false;
    }
    //Also free - even if not needed - and exit
    free(temp);
    return true;
}

bool append_path(char *dir_path, char *append){
    //Get the position of the '\0' in the current path
    size_t end = strlen(dir_path);
    //Check if we will not have overflow
    if (end + strlen(append) >= MAX_DIR_EXPANSION){
        return false;
    }
    //Otherwise do the append
    snprintf(dir_path + end, MAX_DIR_EXPANSION - end, "/%s", append);
    //And return that all went AOK
    return true;
}

void apply_mode(char mode, char *curr_dir)
{
    // '1' is a mode for Normal file mounting
    if (mode == '1') {
        // Start by unmounting possibly mounted SD card
        unmount_sdcard();
        // Attempt mounting the little fs file system
        if (!mount_little_fs()) {
            //Blink the error code
            blink_error(LED_MOUNTING_FAILED);
            //Trigger full system reboot
            abort();
        }
        // Set the current directory as base mount
        snprintf(curr_dir, MAX_MESSAGE_SIZE, LITTLE_FS_BASE_PATH);
    } else if (mode == '2') {
        // '2' Is for mounting on SD external card
        // Start by unmounting possibly mounted Little FS
        unmount_little_fs();
        // Then mount
        if (!mount_sdcard()) {
            //Blink the error code
            blink_error(LED_MOUNTING_FAILED);
            //Trigger full system reboot
            abort();
        }
        // Set the current directory as base mount
        snprintf(curr_dir, MAX_MESSAGE_SIZE, SD_BASE_PATH);
    } else if (mode == '3') {
        // '3' is the mod where we are changing params on the little fs
        // Start by unmounting possibly mounted Little FS
        unmount_little_fs();
        // Then mount
        if (!mount_sdcard()) {
            //Blink the error code
            blink_error(LED_MOUNTING_FAILED);
            //Trigger full system reboot
            abort();
        }
        // Set the current directory as base mount
        snprintf(curr_dir, MAX_MESSAGE_SIZE, SD_BASE_PATH);

        // Change the params

    } else if (mode == '4') {
        // '3' is the benchmarking mode, just prints the stats
        // Start by unmounting possibly mounted SD card
        unmount_sdcard();
        // Attempt mounting the little fs file system
        if (!mount_little_fs()) {
            //Blink the error code
            blink_error(LED_MOUNTING_FAILED);
            //Trigger full system reboot
            abort();
        }
        // Set the current directory as base mount
        snprintf(curr_dir, MAX_MESSAGE_SIZE, LITTLE_FS_BASE_PATH);

        // Run the benchmark
        run_benchmark();
    }
}