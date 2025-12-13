#include "../filesystem.h"

// Includes
#include "imp.h"
#include "sys/dirent.h"

// System Includes
#include <stdio.h>
#include <sys/stat.h>

bool cmd_cat(char *res, char *err, char *curr_dir, const char *args)
{
    // Inform the user of missing argument (must have arg)
    if (!args || args[0] == '\0') {
        snprintf(err, MAX_MESSAGE_SIZE, "cat: No file specified");
        return false;
    }

    // Set the current directory
    char file[MAX_DIR_EXPANSION] = {0};
    // Join the paths
    path_join(file, sizeof(file), curr_dir, args);

    // Try to open (create) the file
    FILE *f = fopen(file, "a");

    // If file not found/opened/created
    if (!f) {
        snprintf(err, MAX_MESSAGE_SIZE, "cat: Failed to find the file.");
        return false;
    }

    // Try to read n bytes from the file
    int read_bytes = (int)fread(res, 1, MAX_MESSAGE_SIZE - 1, f);
    // Set the last char as end of string
    res[(size_t)read_bytes] = '\0';

    // This means it will be printed even if it needs to be truncated

    return true;
}

bool cmd_ls(char *res, char *err, char *curr_dir, const char *args)
{
    // Set the current directory
    char dir[MAX_DIR_EXPANSION];

    // If no argument given: Implicitly means list curr dir
    if (!args || args[0] == '\0') {
        snprintf(dir, sizeof(dir), "%s", curr_dir);
    } else {
        // Otherwise use the args to specify the path
        path_join(dir, sizeof(dir), curr_dir, args);
    }

    // Open the directory (attempt to)
    DIR *directory = opendir(dir);

    // If file not found/opened/created
    if (!directory) {
        snprintf(err, MAX_MESSAGE_SIZE, "ls: Failed to find the directory.");
        return false;
    }

    // Value holders
    int offset = 0;
    struct dirent *list;
    char local_path[MAX_DIR_EXPANSION];

    // Cycle and read the files/dirs in the dir
    while ((list = readdir(directory)) != 0) {
        // Join the path to teh name
        path_join(local_path, MAX_DIR_EXPANSION, dir, list->d_name);
        
        // See if it is directory or just a file
        struct stat path_stat;
        if (stat(local_path, &path_stat) != 0) {
            // Continue if impossible to access data
            // Possible error but ignore it
            continue;
        }
        char type = S_ISDIR(path_stat.st_mode) ? 'd' : 'f';

        int size_free_to_allocate = 0;
        // Get the current, viable size
        if (offset < MAX_MESSAGE_SIZE) {
            size_free_to_allocate = MAX_MESSAGE_SIZE - offset;
        } else {
            // Otherwise zero space to allocate and we can't print more
            size_free_to_allocate = 0;
        }

        // print the statistics of the file/dir
        snprintf(res + (size_t)offset, (size_t)size_free_to_allocate, 
                 "%c %ld %s\n", type, path_stat.st_size, list->d_name);
        
        // Break when overreached allocated space
        if (offset > MAX_MESSAGE_SIZE) {
            break;
        }
    }

    // Close the directory
    closedir(directory);

    return true;
}

bool cmd_pwd(char *res, char *err, char *curr_dir, const char *args)
{
    // Simply return the current directory in the message
    snprintf(res, sizeof(res), "%s", curr_dir);
    // Can't really fail
    return true;
}