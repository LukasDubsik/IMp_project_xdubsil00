#pragma once

/* INCLUDES */
#include <stdbool.h>
#include <string.h>


/* MACROS */
/**
 * @def LOW
 * @brief The GND logical unit.
 */
#define LOW 0
/**
 * @def HIGH
 * @brief The (5V/3.3V) logical unit
 */
#define HIGH 1
/**
 * @def TAG
 * @brief Name used for debugging messages that can be monitored in runtime.
 */
#define TAG "IMP-xdubsil00"
/**
 * @def SYSTEM_INTRO
 * @brief The example of a possible device name for this project.
 */
#define SYSTEM_INTRO "IMP:file_system@"
/**
 * @def SYSTEM_INTRO_LEN
 * @brief Just length of the SYSTEM_INTRO macro
 */
#define SYSTEM_INTRO_LEN strlen(SYSTEM_INTRO)
/**
 * @def WAIT_FOR_PI
 * @brief How long to keep waiting for the pi to send message back (in ms)
 */
#define WAIT_FOR_PI 1000


/* PROTOTYPES */
/**
 * @brief The central function of the project which starts the hardware and everything.
 */
void app_main(void);
/**
 * @brief Functions that calls the setups for individual used hardwares.
 */
void setup_hardware(void);
/**
 * @brief Waits until restart or key in valid pressed, then returns it.
 *
 * @param valid The array of chars that can be accepted when pressed.
 * @param returned Pointer to the received character.
 *
 * @return Returns false if restart key was pressed or true if valid input received.
 */
bool scan_for_selection(const char *valid, char *returned);