#include "../filesystem.h"

// Includes
#include "imp.h"

// System Includes
#include <stdio.h>
#include <sys/stat.h>
#include <sys/unistd.h>

bool cmd_mkdir(char *res, char *err, char *curr_dir, const char *args)
{
    // Check that arg to create is present
    if (!args || args[0] == '\0') {
        snprintf(err, MAX_MESSAGE_SIZE, "mkdir: Missing arg");
        return false;
    }

    // The resulting dir
    char dir[MAX_DIR_EXPANSION] = {0};

    // Join the current dir to the res dir
    path_join(dir, sizeof(dir), curr_dir, args);

    // Try to create the directory loacally
    if (mkdir(dir, DIR_MODE) != 0) {
        snprintf(err, MAX_MESSAGE_SIZE, "mkdir: Failed to create the dir.");
        return false;
    }

    // Don't inform about success -> Not an error return means success
    return true;
}

bool cmd_rmdir(char *res, char *err, char *curr_dir, const char *args)
{
    // Check that arg to create is present
    if (!args || args[0] == '\0') {
        snprintf(err, MAX_MESSAGE_SIZE, "rmdir: Missing arg");
        return false;
    }

    // The resulting dir
    char dir[MAX_DIR_EXPANSION] = {0};

    // Join the current dir to the res dir
    path_join(dir, sizeof(dir), curr_dir, args);

    // Try to remove the directory
    if (rmdir(dir) != 0) {
        snprintf(err, MAX_MESSAGE_SIZE, "rmdir: Failed to remove the dir.");
        return false;
    }

    // Don't inform about success -> Not an error return means success
    return true;
}

bool cmd_touch(char *res, char *err, char *curr_dir, const char *args)
{
    // Check that arg to create is present
    if (!args || args[0] == '\0') {
        snprintf(err, MAX_MESSAGE_SIZE, "touch: Missing arg");
        return false;
    }

    // The resulting dir
    char file[MAX_DIR_EXPANSION] = {0};

    // Join the current dir to the res dir
    path_join(file, sizeof(file), curr_dir, args);

    // Try to open (create) the file
    FILE *f = fopen(file, "a");

    // If file not found/opened/created
    if (!f) {
        snprintf(err, MAX_MESSAGE_SIZE, "touch: Failed to create the file.");
        return false;
    }

    // Close the file
    fclose(f);

    // Don't inform about success -> Not an error return means success
    return true;
}

bool cmd_rm(char *res, char *err, char *curr_dir, const char *args)
{
    // Check that arg to create is present
    if (!args || args[0] == '\0') {
        snprintf(err, MAX_MESSAGE_SIZE, "rm: Missing arg");
        return false;
    }

    // The resulting dir
    char file[MAX_DIR_EXPANSION] = {0};

    // Join the current dir to the res dir
    path_join(file, sizeof(file), curr_dir, args);

    // Try to unlink (remove) the file
    int unlk = unlink(file);

    // If file not unlinked
    if (unlk != 0) {
        snprintf(err, MAX_MESSAGE_SIZE, "rm: Failed to remove the file.");
        return false;
    }

    // Don't inform about success -> Not an error return means success
    return true;
}