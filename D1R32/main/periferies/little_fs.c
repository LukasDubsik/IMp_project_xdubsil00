//User Includes
#include "little_fs.h"

#include "esp_littlefs.h"


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
        abort();
    }
}