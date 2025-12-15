//User Includes
#include "little_fs.h"
#include "led.h"
#include "../imp.h"

//Systemn Includes
#include "esp_littlefs.h"
#include "esp_log.h"

//Standart Includes
#include <errno.h>
#include <sys/unistd.h>


bool mount_little_fs(char *base_path, char *part)
{
    // Return if already mounted
    if (fs_mounted) {
        return true;
    }

    //Set up the struct for mounting properties
    esp_vfs_littlefs_conf_t conf = {
        .base_path = base_path,                         //Starting point - any path starting with this gets rerouted to our mounted partition
        .partition_label = part,                        //Into which set partition of our storage do we mount
        .format_if_mount_failed = true,                 //If boot fails - erase data and retry (good for boot, not good other times)
        .dont_mount = false                             //Mount immediately upon start of the system
    };

    //Register and immediately mount a bare filesystem at the specified partition
    esp_err_t error = esp_vfs_littlefs_register(&conf);
    //If the mounting failed
    if (error != ESP_OK){
        return false;
    }

    fs_mounted = true;

    return true;
}

bool unmount_little_fs(char *part)
{
    // Return just true if not mounted
    if (!fs_mounted) {
        return true;
    }

    // Unmount the filesystem
    esp_vfs_littlefs_unregister(part);
    // Unset the mounted flag
    fs_mounted = false;

    return true;
}