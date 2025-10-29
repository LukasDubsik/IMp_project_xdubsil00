//User defined Includes
#include "commands.h"
#include "../imp.h"
#include "../periferies/little_fs.h"

//System Includes
#include "esp_littlefs.h"
#include "esp_log.h"


bool cmd_df(void){
    //Get information about current system configuration
    size_t total = 0, used = 0;
    int error = esp_littlefs_info(LITTLE_FS_PARTITION_LABEL, &total, &used);
    //Check that the system is stable
    if (error == ESP_OK){
        //Log for debugging
        ESP_LOGI(TAG, "LittleFS mounted at %s (%u KB total, %u KB used)", conf.base_path, total, used);
        //Later will be used when getting system data
    }
}