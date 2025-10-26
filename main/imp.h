#pragma once

/* INCLUDES */


/* MACROS */
#define LOW 0
#define HIGH 1


/* 
//Maps where the pins are for rows
gpio_num_t ROW_PINS[4] = { GPIO_NUM_13, GPIO_NUM_12, GPIO_NUM_14, GPIO_NUM_27 };
//Maps where the pins are for the columns
gpio_num_t COLUMN_PINS[4] = { GPIO_NUM_35, GPIO_NUM_34, GPIO_NUM_36, GPIO_NUM_39 };

//
static uint8_t  stable_state[4][4];      // current accepted (debounced) state: 0=up, 1=down
//
static int64_t  stable_until_us[4][4];   // per-key "do-not-change-before" timestamp

//Store the outline of the Keyboard
const char KEYMAP[4][4] = {
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' }
}; */

void app_main(void);