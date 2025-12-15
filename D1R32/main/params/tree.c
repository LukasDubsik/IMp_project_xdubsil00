#include "params.h"

// Includes
#include "imp.h"
#include "filesystem/filesystem.h"

// System Includes
#include "sys/dirent.h"
#include <sys/stat.h>

void build_tree(const char *tag, const char *curr_dir, int depth, int max_depth)
{
    // Cap the maximum search depth
    if (depth > max_depth) {
        ESP_LOGI(tag, "%*s...", depth*2, "");
        return;
    }

    // Open the current directory
    DIR *directory = opendir(curr_dir);
    // If failing to, inform and exit
    if (!directory) {
        ESP_LOGE(tag, "%*sCannot open: %s", depth * 2, "", curr_dir);
        return;
    }

    // Keeper values
    struct dirent *list = NULL;
    char local_path[MAX_DIR_EXPANSION] = {0};

    // Keep reading from the directory dir/file at a time
    while ((list = readdir(directory)) != NULL) {
        // Skip the . and .. records -> They are implicit
        if (strcmp(list->d_name, ".") == 0 || strcmp(list->d_name, "..") == 0) {
            continue;
        }

        // The full path -> For searching files/setting dirs
        path_join(local_path, MAX_DIR_EXPANSION, curr_dir, list->d_name);

        // Get if we are getting a new a dir or end-of-line file
        struct stat path_stat;
        if (stat(local_path, &path_stat) != 0) {
            // Ignore a possible error but don't print 
            // possibly corrupted file/dir
            continue;
        }

        // Get if we are dealing with a dir/file
        char type = S_ISDIR(path_stat.st_mode) ? 'd' : 'f';
        // Print it based on this
        ESP_LOGI(tag, "%*s%c %ld %s", depth * 2, "", type, 
                 (long)path_stat.st_size, list->d_name);

        // Then apply recursion and descent of a dir
        if (type == 'd') {
            build_tree(tag, local_path, depth + 1, max_depth);
        }
    }

    // Close the current dir after being read through
    closedir(directory);
}