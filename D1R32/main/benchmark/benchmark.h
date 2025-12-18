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
/**
* @struct benchmark_data
* @brief Holds information about individual command benchmark.
*/
typedef struct {
    int64_t time;
    int passed;
    int failed;
    int transferred;
} benchmark_data;

/* PROTOTYPES */
/**
* @brief Runs the benchmark commands given the starting directory.
*
* @details The benchmark consists of taking all the commands currently 
*          available for my filesystems and using them inside the disk 
*          environment. Each command is run 100 times and the statistics 
*          gathered
*
* @param start_dir The name of the starting directory
*/
void run_benchmark(char *start_dir);