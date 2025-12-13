#include "../filesystem.h"

// Includes
#include "imp.h"

// System Includes
#include <stdio.h>
#include <string.h>

bool cmd_mv(char *res, char *err, char *curr_dir, const char *args)
{
    // Check that arg to create is present
    if (!args || args[0] == '\0') {
        snprintf(err, MAX_MESSAGE_SIZE, "mv: Missing args");
        return false;
    }

    // Create a temporary message holder
    char hold[MAX_DIR_EXPANSION] = {0};
    // Copy the arguments into it
    snprintf(hold, sizeof(hold), "%s", args);

    // Save the first and second args -> src - dst
    char *save = NULL;
    char *source = strtok_r(hold, " ", &save);
    char *destination = strtok_r(NULL, " ", &save);

    // Check both params are present
    if (source == NULL || destination == NULL) {
        snprintf(err, MAX_MESSAGE_SIZE, "mv: Expected two args");
        return false;
    }

    // Acquire the full path from root
    char src_path[MAX_DIR_EXPANSION] = {0};
    char dst_path[MAX_DIR_EXPANSION] = {0};
    // Join them to the rest
    path_join(src_path, sizeof(src_path), curr_dir, source);
    path_join(dst_path, sizeof(dst_path), curr_dir, destination);

    // Attempt to rename (move) the file
    int n = rename(src_path, dst_path);
    if (n != 0) {
        snprintf(err, MAX_MESSAGE_SIZE, "mv: failed to move (rename) the file");
        return false;
    }

    return true;
}

static bool write_into_file(char *res, char *err, char *curr_dir, 
                            const char *args, const char *mode)
{
    // Decide what command it is
    char *m = (strcmp(mode, "a") == 0) ? "append" : "write";

    // Check that arg to create is present
    if (!args || args[0] == '\0') {
        snprintf(err, MAX_MESSAGE_SIZE, "%s: Missing args", m);
        return false;
    }

    // Create a temporary message holder
    char hold[MAX_DIR_EXPANSION] = {0};
    // Copy the arguments into it
    snprintf(hold, sizeof(hold), "%s", args);

    // Save the first and second args -> file - text
    char *save = NULL;
    char *file = strtok_r(hold, " ", &save);
    char *text = strtok_r(NULL, " ", &save);

    // Check both params are present
    if (file == NULL || text == NULL) {
        snprintf(err, MAX_MESSAGE_SIZE, "%s: Expected two args", m);
        return false;
    }

    // Set the destination path
    char path[MAX_DIR_EXPANSION] = {0};
    // Join them to the rest
    path_join(path, sizeof(path), curr_dir, file);

    // Try to open the file
    FILE *f = fopen(file, mode);

    // If file not found/opened/created
    if (!f) {
        snprintf(err, MAX_MESSAGE_SIZE, "%s: Failed to find the file.", m);
        return false;
    }

    // Write text into it (mode guarantees how)
    int written_size = (int)fwrite(text, 1, sizeof(text), f);

    // Close the file
    fclose(f);

    // Check we have written what was expected to be written
    if (sizeof(text) != written_size) {
        snprintf(err, MAX_MESSAGE_SIZE, "%s: Failed to write all the data into the file.", m);
        return false;
    }

    return true;
}

bool cmd_write(char *res, char *err, char *curr_dir, const char *args)
{
    return write_into_file(res, err, curr_dir, args, "w");
}

bool cmd_append(char *res, char *err, char *curr_dir, const char *args)
{
    return write_into_file(res, err, curr_dir, args, "a");
}