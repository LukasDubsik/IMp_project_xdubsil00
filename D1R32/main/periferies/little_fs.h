#pragma once

// Includes
#include <stdbool.h>

/* MACROS */
/**
 * @def LITTLE_FS_PARTITION_LABEL
 * @brief Label of the partition into which little fs will be mounted.
 */
#define LITTLE_FS_PARTITION_LABEL "l_fs"
/**
 * @def LITTLE_FS_BASE_PATH
 * @brief Path onto which the little fs system will be mounted.
 */
#define LITTLE_FS_BASE_PATH "/littlefs"


/* PROTOTYPES */
/**
 * @brief Mounts and redirects to the little_fs filesystem.
 */
bool mount_little_fs(char *base_path, char *part);
bool unmount_little_fs(char *part);