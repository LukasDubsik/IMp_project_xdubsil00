#pragma once

/* INCLUDES */
//Standart library includes
#include <stdbool.h>


/* MACROS */
/**
 * @def ERROR_LED_TIME
 * @brief The defined time the led is on for each error blink.
 */
#define ERROR_LED_TIME 500


/* PROTOTYPES */
/**
 * @brief Prepare the Led's hardware.
 *
 * Reset the pin and set the type to output
 */
void configure_led(void);

/**
 * @brief Set the led to given state -> HIGH on(1), LOW off(0)
 *
 * @param state HIGH/LOW if the led should be on or off
 */
void set_led(bool state);

/**
 * @brief Blink the led -> go to on, then wait, then off
 *
 * @param on_time The time the led is supposed to be turned on before turning off
 */
void blink_led(int on_time);

/**
 * @brief Function used for when blinking errors
 *
 * @param error_type The numerical type of the error -> determines how many times  the led flashes for that given error
 */
void blink_error(int error_type);