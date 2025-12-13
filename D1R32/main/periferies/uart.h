#pragma once

/* INCLUDES */
//System Includes
#include "soc/gpio_num.h"

//Atandart Includes
#include <stdbool.h>
#include <stdint.h>


/* MACROS */
/**
 * @def UART_PORT
 * @brief What UART port will be used.
 */
#define UART_PORT UART_NUM_2
/**
 * @def UART_TX
 * @brief GPIO port through to which send the data
 */
#define UART_TX GPIO_NUM_17
/**
 * @def UART_RX
 * @brief GPIO pin through which to receive the data
 */
#define UART_RX GPIO_NUM_16
/**
 * @def UART_BAUD
 * @brief The BAUD rate (taken from recommended values).
 */
#define UART_BAUD 115200
/**
 * @def RX_BUFFER
 * @brief Size of the receiver buffer.
 */
#define RX_BUFFER 1024
/**
 * @def UART_BAUD
 * @brief Size of the sender buffer.
 */
#define TX_BUFFER 2048

/**
 * @def UART_READ_PASSED
 * @brief Error code signifying that all passed correctly and all the data was read.
 */
#define UART_READ_PASSED 0
/**
 * @def UART_READ_TIMEOUT
 * @brief Error code signifying that timeout ocured before all data was received.
 */
#define UART_READ_TIMEOUT 1
/**
 * @def UART_READ_BUFFER_OVERFLOW
 * @brief Error code signifying that too much data in one message and buffer overflow occured.
 */
#define UART_READ_BUFFER_OVERFLOW 2

/**
 * @def START_CMD
 * @brief Start command when sent from Pi4 means it is working
 */
#define START_CMD "start_cmd"
/**
 * @def RST_CMD
 * @brief Reset command when set to Pi4 will cause it to close as client has reset itself
 */
#define RST_CMD "rst_cmd"
/**
 * @def OVERFLOW_CMD
 * @brief Overflow command when sent to Pi will cause it to shutdown
 */
#define OVERFLOW_CMD "overflow_cmd"


/* PROTOTYPES */
/**
 * @brief Setup the uart assymetric communication.
 */
void configure_uart(void);
/**
 * @brief Send a newline through the uart communication
 */
void uart_newline(void);
/**
 * @brief print the prompt as is typical in console for fole systems
 *
 * @param currrent_folder The current folder to be printed
 */
void uart_send_prompt(const char *current_folder);
/**
 * @brief Print given data through the uart communication
 *
 * @param data The data to be sent through.
 */
void uart_send_data(const char *data);
/**
 * @brief Read the message from the UARt receiver array.
 *
 * @param read Array into which the message will be saved - assumed to be zerod
 * @param use_timeout If we should set some time limit for the opeartion to complete
 * @param time_until The time limit until we time out and exit the function unsuccesfully
 *
 * @return True if th message was correctly received, False if overflow or timeout.
 */
char uart_read(char *read, bool use_timeout, int64_t time_until);