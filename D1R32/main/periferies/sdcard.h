#pragma once

// Includes
#include <stdbool.h>

/* MACROS */
/**
 * @def SD_PARTITION_LABEL
 * @brief Label of the partition into which sd card will be mounted.
 */
#define SD_PARTITION_LABEL "sd_card"
/**
 * @def SD_BASE_PATH
 * @brief Path onto which the sdcard system will be mounted.
 */
#define SD_BASE_PATH "/sdcard"

/**
 * @def MAX_FILES_SD
 * @brief Maximum files on sd card
 */
#define MAX_FILES_SD 256
/**
 * @def ALLOC_UNIT_SIZE
 * @brief Size allocation on SD
 */
#define ALLOC_UNIT_SIZE (16*1024)

/**
 * @def SD_MOSI
 * @brief Pin connected for MOSI on the SD card
 */
#define SD_MOSI GPIO_NUM_23
/**
 * @def SD_MISO
 * @brief Pin connected for MISO on the SD card
 */
#define SD_MISO GPIO_NUM_19
/**
 * @def SD_SCK
 * @brief Pin connected for SCK on the SD card
 */
#define SD_SCK GPIO_NUM_18
/**
 * @def SD_CS
 * @brief Pin connected for CS on the SD card
 */
#define SD_CS GPIO_NUM_5


/* PROTOTYPES */
/**
 * @brief Mounts and redirects to the external sdcard.
 */
bool mount_sdcard(void);

/**
 * @brief Unmounts the xternal sdcard.
 */
bool unmount_sdcard(void);