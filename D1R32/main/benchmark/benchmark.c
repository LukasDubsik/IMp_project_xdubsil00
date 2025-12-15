#include "benchmark.h"

// Includes
#include "esp_log.h"
#include "esp_log_timestamp.h"
#include "filesystem/filesystem.h"
#include "imp.h"

// System Includes
#include "esp_timer.h"
#include <stdint.h>
#include <stdio.h>

static const char *TAG_BENCH = "bench";

static void add_bench(benchmark_data *dat, int64_t time, int data, bool pass)
{
    dat->time += time;
    dat->transferred += data;
    if (pass) {
        dat->passed += 1;
    } else {
        dat->failed += 1;
    }
}

static void set_bench(benchmark_data *dat)
{
    dat->time = 0;
    dat->transferred = 0;
    dat->passed = 0;
    dat->failed = 0;
}

// static double bench_throughput(benchmark_data *dat)
// {
//     if (dat->time <= 0) {
//         return 0.0;
//     }
//     // The time in seconds
//     double sec = (double)dat->time/1e6;
//     // The size of data transferred in Megabytes
//     double mib = (double)dat->transferred/(1024.0*1024.0);
//     // Return the thrbugoutput
//     return mib/sec;
// }

void run_benchmark(char *start_dir)
{
    // Setup the holders for errors and messages
    static char res[MAX_MESSAGE_SIZE] = {0};
    static char err_msg[MAX_MESSAGE_SIZE] = {0};

    // Set the current directory
    static char curr_dir[MAX_DIR_EXPANSION] = {0};
    snprintf(curr_dir, MAX_DIR_EXPANSION, "%s", start_dir);

    // Allocate for the argeuments
    //static char args[MAX_MESSAGE_SIZE] = {0};

    // Then start running the benchmark command by command
 
    // 1) Create and remove files
    int run = 0;
    // The trackers
    benchmark_data touch, rm;
    set_bench(&touch);
    set_bench(&rm);

    while (run < TOUCH_RM_BENCH) {
        int64_t t0 = esp_timer_get_time();
        // Create a test file
        bool r = cmd_touch(res, err_msg, curr_dir, "bench.txt");
        // Time after the command
        int64_t t1 = esp_timer_get_time();
        // Break if is false
        if (r == false) {
            ESP_LOGI(TAG_BENCH, "The touch command failed in %d iteration.", run++);
            add_bench(&touch, t1 - t0, 0, false);
            continue;
        }
        // Set the value in tracker
        add_bench(&touch, t1 - t0, 0, true);

        // Immediately r   emove the command
        t0 = esp_timer_get_time();
        r = cmd_rm(res, err_msg, curr_dir, "bench.txt");
        // Time after the command
        t1 = esp_timer_get_time();
        // Break if is false
        if (r == false) {
            ESP_LOGI(TAG_BENCH, "The rm command failed in %d iteration.", run++);
            add_bench(&rm, t1 - t0, 0, false);
            continue;

        }
        // Set the value in tracker
        add_bench(&rm, t1 - t0, 0, true);

        run++;
    }

    // Print the results of the two commands
    ESP_LOGI(TAG_BENCH, "TOUCH: ops=%d fails=%d time=%.3fs avg_time=%.3fs",
             touch.passed, touch.failed, (double)touch.time/1e6, (double)touch.time/(1e6*run));
    ESP_LOGI(TAG_BENCH, "RM: ops=%d fails=%d time=%.3fs avg_time=%.3fs",
             rm.passed, rm.failed, (double)rm.time/1e6, (double)rm.time/(1e6*run));

    
}