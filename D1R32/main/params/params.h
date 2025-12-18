#pragma once

#include "esp_log.h"

// System Includes
#include "esp_vfs_fat.h"
#include "sd_protocol_types.h"
#include "sdmmc_cmd.h"
#include <stdio.h>
#include <sys/_locale.h>

/* FUNCTIONS */
/**
* @brief Print the statistics of external sd disk
*
* @param mount The place where the card is mounted
*/
void disk_stats(const char *mount);
/**
* @brief Print the statistics of chip's disk
*
* @param mount The place where the card is mounted
*/
void littlefs_stats(const char *mount);
/**
* @brief Print the directory tree of the disk.
*/
void build_tree(const char *tag, const char *curr_dir, int depth, int max_depth);