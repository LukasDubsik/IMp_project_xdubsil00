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

    // Split the command for possible args
    char *arg_pos = NULL;
    strtok_r(command, " ", &arg_pos);
    const char *args = (arg_pos != NULL) ? arg_pos : "";

    // Trim the arguments too
    if (args && args[0] != '\0') {
        trim((char *)args);
    }

    // Go command by command and see if the submitted matches to 
    // any of the recognized ones
    if (strcmp(command, "df") == 0) {
        perform_command(cmd_df, curr_dir, args);
    } else if (strcmp(command, "help") == 0) {
        perform_command(cmd_help, curr_dir, args);
    } else if (strcmp(command, "stat") == 0) {
        perform_command(cmd_stat, curr_dir, args);
    } else if (strcmp(command, "cd") == 0) {
        perform_command(cmd_cd, curr_dir, args);
    } else if (strcmp(command, "mkdir") == 0) {
        perform_command(cmd_mkdir, curr_dir, args);
    } else if (strcmp(command, "rmdir") == 0) {
        perform_command(cmd_rmdir, curr_dir, args);
    } else if (strcmp(command, "touch") == 0) {
        perform_command(cmd_touch, curr_dir, args);
    } else if (strcmp(command, "rm") == 0) {
        perform_command(cmd_rm, curr_dir, args);
    } else if (strcmp(command, "pwd") == 0) {
        perform_command(cmd_pwd, curr_dir, args);
    } else if (strcmp(command, "ls") == 0) {
        perform_command(cmd_ls, curr_dir, args);
    } else if (strcmp(command, "cat") == 0) {
        perform_command(cmd_cat, curr_dir, args);
    } else if (strcmp(command, "mv") == 0) {
        perform_command(cmd_mv, curr_dir, args);
    } else if (strcmp(command, "write") == 0) {
        perform_command(cmd_write, curr_dir, args);
    } else if (strcmp(command, "append") == 0) {
        perform_command(cmd_append, curr_dir, args);
    } else {
        uart_send_data("Unknown command");
    }
    
    //Then print the command prompt
    uart_send_prompt(curr_dir);
}

void perform_command(bool (*command)(char *, char *, char *, const char *),
                     char *curr_dir, const char *args){
    //Allocate the necessary space for the possible result of the command
    static char res[MAX_MESSAGE_SIZE];
    //Also allocate for possible error message
    static char err_msg[MAX_MESSAGE_SIZE];

    // Set the start char
    res[0] = '\0';
    err_msg[0] = '\0';

    //Perform the command and get if passed/failed
    bool err = command(res, err_msg, curr_dir, args);

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

void path_join(char *out, int size, const char *base, const char *rel)
{
    // If the path starts with '/' treat as absolute path
    // The absolute path is always under "/littlefs"
    // No changing between possible disks (for now disabled)
    if (rel[0] == '/') {
        snprintf(out, (size_t)size, "%s%s", LITTLE_FS_BASE_PATH, rel);
        return;
    }

    // If the path ends with '/'
    if (base[strlen(base) - 1] == '/') {
        snprintf(out, (size_t)size, "%s%s", base, rel);
    } else {
        // If it doesn't, add it
        snprintf(out, (size_t)size, "%s/%s", base, rel);
    }
}