#pragma once

// System Includes
#include <stdint.h>

/* MACROS */
/**
* @brief How many times to create/remove a file
*/
#define TOUCH_RM_BENCH 100

/* STRUCTS */
typedef struct {
    int64_t time;
    int passed;
    int failed;
    int transferred;
} benchmark_data;

/* PROTOTYPES */
void run_benchmark(void);