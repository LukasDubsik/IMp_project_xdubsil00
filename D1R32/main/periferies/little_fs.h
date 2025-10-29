#pragma once

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
void mount_little_fs(void);