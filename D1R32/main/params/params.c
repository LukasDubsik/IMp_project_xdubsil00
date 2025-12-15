#include "params.h"
#include "esp_littlefs.h"
#include "imp.h"
#include "periferies/little_fs.h"

void disk_stats(const char *mount)
{
    // Delimiter
    ESP_LOGI(TAG, "");

    // Define the testing tag
    char tag_loc[256] = {0};
    snprintf(tag_loc, 256, "EXTERNAL SD");

    // Start the testing output
    ESP_LOGI(tag_loc, "%s:", tag_loc);
    ESP_LOGI(tag_loc, "-> CARD INFO <-");

    // Get the base card details
    // Directly prints to the output
    sdmmc_card_print_info(stdout, sd_card);

    // Acquire the filesystem data (empty/full/used/...)
    uint64_t total = 0;
    uint64_t free = 0;

    // Attempt to gather the data
    // Exit if unssucessfull -> Continuing is meaningless as there is 
    // clearly an error with the mounted card/system
    if (esp_vfs_fat_info(mount, &total, &free) != ESP_OK) {
        ESP_LOGE(tag_loc, "Failed to get filesystem info for: %s!", mount);
        return;
    }

    // Also inform of the usage
    uint64_t used = (total > free) ? (total - free) : 0;

    // Then write of that info
    ESP_LOGI(tag_loc, "-> FATFS <-");
    ESP_LOGI(tag_loc, "There are currently %" PRIu64 " KiB of which %" PRIu64 " are free.",
             total/1024ULL,free/ 1024ULL);
    
    // Also inform of the usage
    ESP_LOGI(tag_loc, "Used KiB are %" PRIu64 ".", used/1024ULL);

    // Lastly generate the directory tree
    ESP_LOGI(tag_loc, "-> DIR TREE <-");
    build_tree(tag_loc, mount, 0, 5);
}

void littlefs_stats(const char *mount)
{
    // Delimiter
    ESP_LOGI(TAG, "");

    // Define the testing tag
    char tag_loc[256] = {0};
    snprintf(tag_loc, 256, "LITTLE FS");

    // Start the testing output
    ESP_LOGI(tag_loc, "%s:", tag_loc);
    ESP_LOGI(tag_loc, "-> SYSTEM INFO <-");

    // Try and locate the partition's location
    const esp_partition_t *part = esp_partition_find_first(
        ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, mount);

    // Check it exists
    // If not found, no reason to continue
    if (part == NULL) {
        ESP_LOGE(tag_loc, "Not found the partition %s!", mount);
        return;
    }

    // Print its data
    ESP_LOGI(tag_loc, "Label == %s;", part->label);
    ESP_LOGI(tag_loc, "Address == 0x%" PRIx32 ";", part->address);
    ESP_LOGI(tag_loc, "Size == %" PRIu32 ";", part->size);
    ESP_LOGI(tag_loc, "B Encrypted == %d;", (int)part->encrypted);

    // Then acquire the current usage statistics
    size_t total = 0;
    size_t used  = 0;

    // Attempt to gather the data
    // Exit if unssucessfull -> Continuing is meaningless as there is 
    // clearly an error with the mounted card/system
    if (esp_littlefs_info(mount, &total, &used) != ESP_OK) {
        ESP_LOGE(tag_loc, "Failed to gather usage data for %s", mount);
        return;
    }

    // Then print them
    ESP_LOGI(tag_loc, "-> LITTLE FS <-");
    ESP_LOGI(tag_loc, "There are currently %" PRIu64 "KiB of which %" PRIu64 " KiB are used.",
             total/1024ULL, used/1024ULL);

    // Lastly generate the directory tree
    ESP_LOGI(tag_loc, "-> DIR TREE <-");
    build_tree(tag_loc, LITTLE_FS_BASE_PATH, 0, 5);
}