#pragma once

// System Includes
#include <stdint.h>

/* MACROS */
/**
* @brief How many times to create/remove a file
*/
#define TOUCH_RM_BENCH 100
/**
 * @def BENCHMARK_BASE_PATH
 * @brief Base path for benchmark's starting directory
 */
#define BENCHMARK_BASE_PATH "/benchmark"
/**
 * @def BENCHMARK_PARTITION_LABEL
 * @brief Name of the partition label on which to run benchmark
 */
#define BENCHMARK_PARTITION_LABEL "b_fs"

/* STRUCTS */
typedef struct {
    int64_t time;
    int passed;
    int failed;
    int transferred;
} benchmark_data;

/* PROTOTYPES */
void run_benchmark(char *start_dir);