#pragma once

/* INCLUDES */
//User defined Includes
#include "periferies/uart.h"

//Standart Includes
#include <stdbool.h>
#include <string.h>


/* Global variables */
/**
 * @def restart_pressed
 * @brief Tracks if the restart char ('6') was pressed
 */
extern volatile bool restart_pressed;


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
#define SYSTEM_INTRO "IMP@file_system:"
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
/**
 * @def MAX_DIR_EXPANSION
 * @brief Maximum number of chars the current directory path can contain
 */
#define MAX_DIR_EXPANSION 512
/**
 * @def MAX_MESSAGE_SIZE
 * @brief Maximum number of chars that can be in message sent to PI4 - equivalent to our send buffer size minus safe constant.
 */
#define MAX_MESSAGE_SIZE (TX_BUFFER - 32)
/**
 * @def MAX_RECEIVE_SIZE
 * @brief Maximum number of chars that can be returned by PI4.
 */
#define MAX_RECEIVE_SIZE RX_BUFFER


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
/**
 * @brief Set the new directory path.
 *
 * @param dir_path String keeping the general value of the current directory.
 * @param new_path String containing the new path to change to.
 *
 * @return True if new path was set correctly, false if failed - the path remains unchanged.
 */
bool set_new_directory_path(char *dir_path, const char *new_path);
/**
 * @brief Append a value to the current path - also adds '/ automatically.
 *
 * @param dir_path String keeping the general value of the current directory.
 * @param append value to append to the current path.
 *
 * @return True if new path was set correctly, false if failed - the path remains unchanged.
 */
bool append_path(char *dir_path, char *append);