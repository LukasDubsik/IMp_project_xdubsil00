#pragma once

/* INCLUDES */
//Standart library includes
#include <stdbool.h>


/* MACROS */
/**
 * @def DEBOUNCE
 * @brief How long to wait for output to stay stable (unchanging)
 *        If change comes after this time has passed -> accept it. Otherwise prolong the wait and l;eave the current value.
 *        Waits 20 ms.
 */
#define ERROR_LED_TIME 500


/* PROTOTYPES */
/**
 * @brief Set the led to given state -> HIGH on(1), LOW off(0)
 *
 * @param state HIGH/LOW if the led should be on or off
 */
void set_led(bool state);

/**
 * @brief Prepare the Led's hardware.
 *
 * Reset the pin and set the type to output
 */
void configure_led(void);