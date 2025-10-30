//User defined Includes
#include "commands.h"
#include "../imp.h"
#include "../periferies/little_fs.h"
#include "../helper_functions/helper_functions.h"

//System Includes
#include "esp_littlefs.h"
#include "esp_log.h"


void select_command(char *command){
    //Strip the trailing whitespaces
    trim(command);
    //Go command by command and see if the submitted matches to any of the recognized ones
    if (strcmp(command, "df")){
        //perform_command();
    }
}

bool cmd_df(char *res){
    //Get information about current system configuration
    size_t total = 0, used = 0;
    int error = esp_littlefs_info(LITTLE_FS_PARTITION_LABEL, &total, &used);
    //Check that the system is stable
    if (error == ESP_OK){
        //Save the message to the returning array
        snprintf(res, MAX_MESSAGE_SIZE, "LittleFS mounted at %s (%u KB total, %u KB used)", LITTLE_FS_BASE_PATH, total, used);
        return true;
    }
    return false;
}