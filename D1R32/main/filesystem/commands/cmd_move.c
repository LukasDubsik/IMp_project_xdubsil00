#include "../filesystem.h"

// Includes
#include "imp.h"
#include "periferies/little_fs.h"

// System Includes
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

bool cmd_cd(char *res, char *err, char *curr_dir, const char *args)
{   
    // The resulting dir
    char dir[MAX_DIR_EXPANSION] = {0};

    // If no destination -> Move to the root
    if (!args || args[0] == '\0') {
        snprintf(dir, sizeof(dir), "%s", LITTLE_FS_BASE_PATH);
        return true;
    }

    // Guard for root access
    if (args[0] == '/') {
        if (strcmp(args, "/") == 0) {
            snprintf(dir, sizeof(dir), "%s", LITTLE_FS_BASE_PATH);
        } else {
            snprintf(dir, sizeof(dir), "%s%s", LITTLE_FS_BASE_PATH, args);
        }
    } else {
        path_join(dir, sizeof(dir), curr_dir, args);
    }

    // Make sure we are truly entering possible new dir
    // To not create unstable state where we print to the user
    // nonexistent dir
    struct stat dir_stat;

    // Guard against possibility of nonexistent path
    if (stat(dir, &dir_stat) != 0) {
        snprintf(err, MAX_MESSAGE_SIZE, "cd: Not a file or directory.");
        return false; 
    }
    // Guard against possibility of not being a dir but a file
    if (!S_ISDIR(dir_stat.st_mode)) {
        snprintf(err, MAX_MESSAGE_SIZE, "cd: Not a directory.");
        return false;
    }

    // Resolve for the real path
    if (realpath(dir, curr_dir) != NULL) {
        return true;
    } else {
        snprintf(err, MAX_MESSAGE_SIZE, "cd: Failed to resolve the path.");
        return false;
    }
}