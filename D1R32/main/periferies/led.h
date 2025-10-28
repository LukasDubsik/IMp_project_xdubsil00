#pragma once

/* INCLUDES */
//Standart library includes
#include <stdbool.h>


/* MACROS */
/**
 * @def ERROR_LED_TIME
 * @brief The defined time the led is on for each error blink (in ms).
 */
#define ERROR_LED_TIME 100
/**
 * @def ERROR_REST_TIME
 * @brief How lomng should the led rest between error blinks (in ms).
 */
#define ERROR_REST_TIME 200
/**
 * @def CORRECT_LED_TIME
 * @brief Used when something passes to inform the user.
 */
#define CORRECT_LED_TIME 1000

/**
 * @def INCORRECT_KEY_TYPED
 * @brief Number of times the LED should flash when incorrect key was typed.
 */
#define INCORRECT_KEY_TYPED 2


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