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
#define LED_INCORRECT_KEY_TYPED 2
/**
 * @def LED_MOUNTING_FAILED
 * @brief Number of times the LED should flash when mounting of the filessytem failed.
 */
#define LED_MOUNTING_FAILED 3
/**
 * @def LED_BUFFER_READ_OVERFLOW
 * @brief Number of times the LED should flash when buffer overflowed during uart read.
 */
#define LED_BUFFER_READ_OVERFLOW 4
/**
 * @def LED_READ_TIMEOUT
 * @brief Number of times the LED should flash when timeout occured during uart read.
 */
#define LED_READ_TIMEOUT 5
/**
 * @def LED_UNEXPECTED_MESSAGE
 * @brief Number of times the LED should flash when unexpected data are recived.
 */
#define LED_UNEXPECTED_MESSAGE 3


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