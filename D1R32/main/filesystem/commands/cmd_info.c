#include "../filesystem.h"

// System Includes
#include "../../imp.h"
#include "../../periferies/little_fs.h"

// Includes
#include "esp_littlefs.h"
#include <stdio.h>
#include <sys/stat.h>

bool cmd_help(char *res, char *err, char *curr_dir, const char *args)
{
    // Set the helper message
    snprintf(res, MAX_MESSAGE_SIZE,
        "Commands:\n"
        "   help               Show this help\n"
        "   df                 Filesystem data\n"
        "   pwd                Print current working directory\n"
        "   ls [path]          List directory\n"
        "   cd <path>          Change directory\n"
        "   mkdir <dir>        Create directory\n"
        "   rmdir <dir>        Remove empty directory\n"
        "   touch <file>       Create file\n"
        "   rm <file>          Remove file (doesn't include -r, includes -f)\n"
        "   cat <file>         Print file\n"
        "   vim <f>            Edit the file (vim emulator)\n"
        "   mv <src> <dst>     Rename/move\n"
        "   stat <path>        File info\n"
    );

    // Always will succeed
    return true;
}

bool cmd_df(char *res, char *err, char *curr_dir, const char *args)
{
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

bool cmd_stat(char *res, char *err, char *curr_dir, const char *args)
{
    // Inform the user of missing argument (must have arg)
    if (!args || args[0] == '\0') {
        snprintf(err, MAX_MESSAGE_SIZE, "No file specified");
        return false;
    }

    // Set the current directory
    char dir[MAX_DIR_EXPANSION] = {0};
    // Join the paths
    path_join(dir, sizeof(dir), curr_dir, args);

    // Acquire the file statistics
    struct stat file_stat;
    // If no file statistics, set an error
    if (stat(dir, &file_stat) != 0) { 
        snprintf(err, MAX_MESSAGE_SIZE, "Couldn't get file statistics.");
        return false; 
    }

    // The type of the statistics being presented
    const char *type = S_ISDIR(file_stat.st_mode) ? "dir" : "file";

    // Print the basic statistics
    snprintf(res, MAX_MESSAGE_SIZE,
        "Path: %s\nType: %s\nSize: %ld\nMode: %o",
        dir, type, file_stat.st_size, file_stat.st_mode & 0777
    );

    return true;
}