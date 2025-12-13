#include "sdcard.h"

// Includes
#include "driver/sdspi_host.h"
#include "hal/spi_types.h"
#include "imp.h"

// System Includes
#include "esp_err.h"
#include "esp_log.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"


bool mount_sdcard(void)
{
    // Check if already mounted
    if (sd_mounted) {
        return true;
    }

    // Set up the struct for mounting properties
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = MAX_FILES_SD,
        .allocation_unit_size = ALLOC_UNIT_SIZE,
    };

    // The base values
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = VSPI_HOST;

    // Configure the bus for communication for the sd card
    // This uses the Hardware connected pins on the chip/sd card
    spi_bus_config_t bus_cfg = {
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4096,
        .mosi_io_num = SD_MOSI,
        .miso_io_num = SD_MISO,
        .sclk_io_num = SD_SCK,
    };

    // Initialize the bus connection
    if (spi_bus_initialize((spi_host_device_t)host.slot, &bus_cfg, SPI_DMA_CH_AUTO) != ESP_OK) {
        ESP_LOGI(TAG, "BUS for SD card initialization failed!");
        return false;
    }

    // Preset the values for mounting
    sdspi_device_config_t slot = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot.gpio_cs = SD_CS;
    slot.host_id = (spi_host_device_t)host.slot;

    if (esp_vfs_fat_sdspi_mount(SD_BASE_PATH, &host,
        &slot, &mount_config, &sd_card) != ESP_OK) {
        ESP_LOGI(TAG, "Failed mounting the SD card!");
        // Free the allocated bus
        spi_bus_free((spi_host_device_t)host.slot);
        return false;
    }

    // We have mounted the sd card
    sd_mounted = true;
    return true;
}

bool unmount_sdcard(void)
{
    // Return just true if not mounted
    if (!sd_mounted) {
        return true;
    }

    // Unmount the card
    esp_vfs_fat_sdcard_unmount(SD_BASE_PATH, sd_card);
    // Unset the values to symbolize unmounted values
    sd_card = NULL;
    sd_mounted = false;
    // Then also unset the bus
    spi_bus_free(VSPI_HOST);

    return true;
}