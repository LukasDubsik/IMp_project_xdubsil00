//User Includes
#include "little_fs.h"
#include "led.h"
#include "../imp.h"

//Systemn Includes
#include "esp_littlefs.h"
#include "esp_log.h"

//Standart Includes
#include <sys/unistd.h>


void mount_little_fs(void)
{
    //Set up the struct for mounting properties
    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/littlefs",           //Starting point - any path starting with this gets rerouted to our mounted partition
        .partition_label = "l_fs",          //Into which set partition of our storage do we mount
        .format_if_mount_failed = true,     //If boot fails - erase data and retry (good for boot, not good other times)
        .dont_mount = false                 //Mount immediately upon start of the system
    };

    //Register and immediately mount a bare filesystem at the specified partition
    esp_err_t error = esp_vfs_littlefs_register(&conf);
    //If the mounting failed
    if (error != ESP_OK){
        //Blink the error code
        blink_error(LED_MOUNTING_FAILED);
        //Trigger full system reboot
        abort();
    }

    //Get information about current system configuration
    size_t total = 0, used = 0;
    error = esp_littlefs_info(conf.partition_label, &total, &used);
    //Check that the system is stable
    if (error == ESP_OK){
        //Log for debugging
        ESP_LOGI(TAG, "LittleFS mounted at %s (%u KB total, %u KB used)", conf.base_path, total, used);
        //Later will be used when getting system data
    }

    //Start at this directory when starting system
    if (chdir("/littlefs") != 0) {
        //Blink the error code
        blink_error(LED_MOUNTING_FAILED);
        //Trigger full system reboot
        abort();
    }
}