#pragma once

/* INCLUDES */
//System Includes
#include "driver/gpio.h"


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


/* PROTOTYPES */
/**
 * @brief Setup teh uart for communication with the PI4 chip.
 */
void configure_uart(void);