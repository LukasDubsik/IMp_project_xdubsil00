//User defined Includes
#include "uart.h"

//System Includes
#include "driver/uart.h"
#include "hal/uart_types.h"

void configure_uart(void)
{
    //Set the properties of this uart
    const uart_config_t cfg = {
        .baud_rate = UART_BAUD,                 //Given in header and taken as recommended value
        .data_bits = UART_DATA_8_BITS,          //Sending by chars
        .parity    = UART_PARITY_DISABLE,       //No parity necessary - assuming communication without an error
        .stop_bits = UART_STOP_BITS_1,          //Standart stop value
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE   //Flow control controlled by the program, disable hardware interferance
    };

    //Setup the third of the UARTS (safer, 0 normally for flashing, 1 may be used by other processes)
    ESP_ERROR_CHECK(uart_param_config(UART_PORT, &cfg));
    //Set what port is receive (rx) and what is send (tx)
    ESP_ERROR_CHECK(uart_set_pin(UART_PORT, UART_TX, UART_RX,UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    //Set the buffer sizes - the send is larger as we are sending the whole file system
    //The recive can be smaller, we are just receiving commands
    ESP_ERROR_CHECK(uart_driver_install(UART_PORT, 1024, 2048, 0, NULL, 0));
}