#include "benchmark.h"

// Includes
#include "esp_log.h"
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

void run_benchmark(char *start_dir)
{
    // Setup the holders for errors and messages
    static char res[MAX_MESSAGE_SIZE] = {0};
    static char err_msg[MAX_MESSAGE_SIZE] = {0};

    // Set the current directory
    static char curr_dir[MAX_DIR_EXPANSION] = {0};
    snprintf(curr_dir, MAX_DIR_EXPANSION, "%s", start_dir);

    // Allocate for the argeuments
    static char args[MAX_MESSAGE_SIZE] = {0};

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
    ESP_LOGI(TAG_BENCH, "TOUCH: suc=%d fails=%d time=%.3fs avg_time=%.3fs",
             touch.passed, touch.failed, (double)touch.time/1e6, (double)touch.time/(1e6*run));
    ESP_LOGI(TAG_BENCH, "RM: suc=%d fails=%d time=%.3fs avg_time=%.3fs",
             rm.passed, rm.failed, (double)rm.time/1e6, (double)rm.time/(1e6*run));


    // 2) Create and remove directories (mkdir/rmdir)
    run = 0;
    // Holders of the benchmark data
    benchmark_data mkdir_b, rmdir_b;
    set_bench(&mkdir_b);
    set_bench(&rmdir_b);

    // Iterate until we reach the end of number of data gathering
    while (run < TOUCH_RM_BENCH) {
        // Set the benchmark name
        char dname[64] = {0};
        snprintf(dname, sizeof(dname), "bench_dir_%d", run);

        // Data for creating a directory
        int64_t t0 = esp_timer_get_time();
        bool r = cmd_mkdir(res, err_msg, curr_dir, dname);
        int64_t t1 = esp_timer_get_time();
        if (r == false) {
            ESP_LOGI(TAG_BENCH, "The mkdir command failed in %d iteration.", run++);
            add_bench(&mkdir_b, t1 - t0, 0, false);
            continue;
        }
        add_bench(&mkdir_b, t1 - t0, 0, true);

        // Followed by a data for deleting a directory
        t0 = esp_timer_get_time();
        r = cmd_rmdir(res, err_msg, curr_dir, dname);
        t1 = esp_timer_get_time();
        if (r == false) {
            ESP_LOGI(TAG_BENCH, "The rmdir command failed in %d iteration.", run++);
            add_bench(&rmdir_b, t1 - t0, 0, false);
            continue;
        }
        add_bench(&rmdir_b, t1 - t0, 0, true);

        run++;
    }

    // Print the resulting benchmarking data
    ESP_LOGI(TAG_BENCH, "MKDIR: suc=%d fails=%d time=%.3fs avg_time=%.3fs",
             mkdir_b.passed, mkdir_b.failed, (double)mkdir_b.time/1e6, (double)mkdir_b.time/(1e6*run));
    ESP_LOGI(TAG_BENCH, "RMDIR: suc=%d fails=%d time=%.3fs avg_time=%.3fs",
             rmdir_b.passed, rmdir_b.failed, (double)rmdir_b.time/1e6, (double)rmdir_b.time/(1e6*run));
    
    // 3) Write / Append data
    run = 0;
    // The benchmark data holders
    benchmark_data wr, ap;
    set_bench(&wr);
    set_bench(&ap);

    // Ensure files exis
    cmd_rm(res, err_msg, curr_dir, "bench_w.txt");
    cmd_rm(res, err_msg, curr_dir, "bench_a.txt");
    cmd_touch(res, err_msg, curr_dir, "bench_a.txt");

    // Create a payload
    char *PAYLOAD = "ABCDEFGH";
    int payload_len = (int)strlen(PAYLOAD);

    // Rn the number of iterations
    while (run < TOUCH_RM_BENCH) {
        // Write to the file
        snprintf(args, sizeof(args), "bench_w.txt %s", PAYLOAD);

        // Gather data for writing 
        int64_t t0 = esp_timer_get_time();
        bool r = cmd_write(res, err_msg, curr_dir, args);
        int64_t t1 = esp_timer_get_time();
        if (r == false) {
            ESP_LOGI(TAG_BENCH, "The write command failed in %d iteration.", run++);
            add_bench(&wr, t1 - t0, 0, false);
            continue;
        }
        add_bench(&wr, t1 - t0, payload_len, true);

        // Set the param for appending
        snprintf(args, sizeof(args), "bench_a.txt %s", PAYLOAD);

        // Gather data for appending
        t0 = esp_timer_get_time();
        r = cmd_append(res, err_msg, curr_dir, args);
        t1 = esp_timer_get_time();
        if (r == false) {
            ESP_LOGI(TAG_BENCH, "The append command failed in %d iteration.", run++);
            add_bench(&ap, t1 - t0, 0, false);
            continue;
        }
        add_bench(&ap, t1 - t0, payload_len, true);

        run++;
    }

    // Print the results
    ESP_LOGI(TAG_BENCH, "WRITE: suc=%d fails=%d bytes=%d time=%.3fs avg_time=%.3fs",
             wr.passed, wr.failed, wr.transferred, (double)wr.time/1e6, (double)wr.time/(1e6*run));
    ESP_LOGI(TAG_BENCH, "APPEND: suc=%d fails=%d bytes=%d time=%.3fs avg_time=%.3fs",
             ap.passed, ap.failed, ap.transferred, (double)ap.time/1e6, (double)ap.time/(1e6*run));

    // Cleanup
    cmd_rm(res, err_msg, curr_dir, "bench_w.txt");
    cmd_rm(res, err_msg, curr_dir, "bench_a.txt");

    // 4) View commands (cat, ls, pwd)
    // Make sure the setup is empty
    cmd_rm(res, err_msg, curr_dir, "bench_cat.txt");
    // Set the payload for cat
    snprintf(args, sizeof(args), "bench_cat.txt %s", PAYLOAD);
    // Create a file for cat and write into it the base payload
    if (cmd_write(res, err_msg, curr_dir, args) == false) {
        ESP_LOGE(TAG_BENCH, "The write command failed for creating starting point for view commands.");
        return;
    }

    run = 0;
    // Data holders of the benchmark
    benchmark_data cat, ls, pwd;
    set_bench(&cat);
    set_bench(&ls);
    set_bench(&pwd);

    // Setup directories for the LS
    cmd_mkdir(res, err_msg, curr_dir, "bench_ls");
    cmd_touch(res, err_msg, curr_dir, "bench_ls/a.txt");
    cmd_touch(res, err_msg, curr_dir, "bench_ls/b.txt");
    cmd_touch(res, err_msg, curr_dir, "bench_ls/c.txt");

    while (run < TOUCH_RM_BENCH) {
        // cat data
        int64_t t0 = esp_timer_get_time();
        bool r = cmd_cat(res, err_msg, curr_dir, "bench_cat.txt");
        int64_t t1 = esp_timer_get_time();
        if (r == false) {
            ESP_LOGI(TAG_BENCH, "The cat command failed in %d iteration.", run++);
            add_bench(&cat, t1 - t0, 0, false);
            continue;
        }
        add_bench(&cat, t1 - t0, payload_len, true);

        // ls data
        t0 = esp_timer_get_time();
        r = cmd_ls(res, err_msg, curr_dir, "bench_ls");
        t1 = esp_timer_get_time();
        if (r == false) {
            ESP_LOGI(TAG_BENCH, "The ls command failed in %d iteration.", run++);
            add_bench(&ls, t1 - t0, 0, false);
            continue;
        }
        add_bench(&ls, t1 - t0, 0, true);

        // pwd data
        t0 = esp_timer_get_time();
        r = cmd_pwd(res, err_msg, curr_dir, "");
        t1 = esp_timer_get_time();
        if (r == false) {
            ESP_LOGI(TAG_BENCH, "The pwd command failed in %d iteration.", run++);
            add_bench(&pwd, t1 - t0, 0, false);
            continue;
        }
        add_bench(&pwd, t1 - t0, 0, true);

        run++;
    }

    // Print the results
    ESP_LOGI(TAG_BENCH, "CAT: suc=%d fails=%d bytes=%d time=%.3fs avg_time=%.3fs",
             cat.passed, cat.failed, cat.transferred, (double)cat.time/1e6, (double)cat.time/(1e6*run));
    ESP_LOGI(TAG_BENCH, "LS: suc=%d fails=%d time=%.3fs avg_time=%.3fs",
             ls.passed, ls.failed, (double)ls.time/1e6, (double)ls.time/(1e6*run));
    ESP_LOGI(TAG_BENCH, "PWD: suc=%d fails=%d time=%.3fs avg_time=%.3fs",
             pwd.passed, pwd.failed, (double)pwd.time/1e6, (double)pwd.time/(1e6*run));

    // Cleanup view setup
    cmd_rm(res, err_msg, curr_dir, "bench_cat.txt");
    cmd_rm(res, err_msg, curr_dir, "bench_ls/a.txt");
    cmd_rm(res, err_msg, curr_dir, "bench_ls/b.txt");
    cmd_rm(res, err_msg, curr_dir, "bench_ls/c.txt");
    cmd_rmdir(res, err_msg, curr_dir, "bench_ls");
}