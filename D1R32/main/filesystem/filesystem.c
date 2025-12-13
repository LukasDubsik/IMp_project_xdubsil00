//User defined Includes
#include "filesystem.h"
#include "../imp.h"
#include "../periferies/little_fs.h"
#include "../helper_functions/helper_functions.h"

//System Includes
#include "esp_littlefs.h"
#include "periferies/uart.h"

//Standart Includes
#include <string.h>


void select_command(char *command, char *curr_dir){
    //Strip the trailing whitespaces
    trim(command);

    // When nothing was received
    if (command[0] == '\0') {
        uart_send_prompt(curr_dir);
        return;
    }

    //Go command by command and see if the submitted matches to any of the recognized ones
    if (strcmp(command, "df") == 0){
        perform_command(cmd_df);
    } else {
        uart_send_data("Unknown command");
    }
    
    //Then print the command prompt
    uart_send_prompt(curr_dir);
}

void perform_command(bool (*command)(char *, char *)){
    //Allocate the necessary space for the possible result of the command
    static char res[MAX_MESSAGE_SIZE];
    //Also allocate for possible error message
    static char err_msg[MAX_MESSAGE_SIZE];

    // Set the start char
    res[0] = '\0';
    err_msg[0] = '\0';

    //Perform the command and get if passed/failed
    bool err = command(res, err_msg);

    //Check if the command passed
    if (err == false){
        //Print to the terminal why it failed
        uart_send_data(err_msg);
    }
    else{
        //Otherwise send the results
        uart_send_data(res);
    }
}

bool cmd_df(char *res, char *err){
    //Get information about current system configuration
    size_t total = 0, used = 0;
    int error = esp_littlefs_info(LITTLE_FS_PARTITION_LABEL, &total, &used);
    //Check that the system is stable
    if (error == ESP_OK){
        //Save the message to the returning array
        snprintf(res, MAX_MESSAGE_SIZE, "LittleFS mounted at %s (%u KB total, %u KB used)", LITTLE_FS_BASE_PATH, total, used);
        return true;
    }
    //Allocate error message
    snprintf(err, MAX_MESSAGE_SIZE, "Couldn't get statistics about the mounted system, possible internal error.");
    return false;
}