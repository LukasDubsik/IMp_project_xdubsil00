#pragma once


/* MACROS */
/**
 * @def DEBOUNCE
 * @brief How long to wait for output to stay stable (unchanging)
 *        If change comes after this time has passed -> accept it. Otherwise prolong the wait and l;eave the current value.
 *        Waits 20 ms.
 */
#define DEBOUNCE 20000
/**
 * @def RT_KEY
 * @brief Key that when pressed restarts the whole program - moves to the start;
 */
#define RESTART_KEY '6'


/* PROTOTYPES */
/**
 * @brief Set the keyboard to be used.
 *
 * Sets the pins, their types and how they are to be operated. Sets for the output all high before scanning.
 */
void configure_keyboard(void);

/**
 * @brief Scan the matrix for possible inputs
 *
 * Go column by column and always set the voltage to GND, then scan all the rows (here only the first two, as not enough pins on microcontroller)
 * Also works to prevent the bounce and false input scanning.
 *
 * @return Returns the pressed character on the keyboard if any key is down. Otherwise returns '\0'
 */
char scan_keyboard(void);

/**
 * @brief Wait until a permisible key is pressed, then pass that pressed key.
 *
 * The result can either be the key given in the valid array or a reset key that makes the whole program start from the beginning again.
 *
 * @return Returns the pressed key on the matrix.
 */
char wait_for_press(const char* valid);