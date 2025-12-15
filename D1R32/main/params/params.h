#pragma once

#include "esp_log.h"

// System Includes
#include "esp_vfs_fat.h"
#include "sd_protocol_types.h"
#include "sdmmc_cmd.h"
#include <stdio.h>
#include <sys/_locale.h>

/* FUNCTIONS */
void disk_stats(const char *mount);

void littlefs_stats(const char *mount);

void build_tree(const char *tag, const char *curr_dir, int depth, int max_depth);