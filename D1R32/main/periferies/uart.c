//User defined Includes
#include "uart.h"

//System Includes
#include "driver/uart.h"
#include "hal/uart_types.h"
#include "imp.h"
#include <string.h>

void configure_uart(void)
{
    //Set holder of properties for the UART
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
    ESP_ERROR_CHECK(uart_driver_install(UART_PORT, RX_BUFFER, TX_BUFFER, 0, NULL, 0));
}

void uart_newline(void){
    //String for more safer transfer
    const char nl[2] = "\n";
    uart_write_bytes(UART_PORT, nl, 1);
}

void uart_send_prompt(const char *current_folder)
{
    //Write the base system prompt
    uart_write_bytes(UART_PORT, SYSTEM_INTRO, SYSTEM_INTRO_LEN);
    //Then write the current folder
    uart_write_bytes(UART_PORT, current_folder, strlen(current_folder));
    //Then write the '$ ' - inspired by ubuntu
    uart_write_bytes(UART_PORT, "$ ", 2);
    //Then write the newline char
    uart_newline();
}

void uart_send_data(const char *data)
{
    //Write the given data
    uart_write_bytes(UART_PORT, data, strlen(data));
    //Then a newline
    uart_newline();
}

bool read_uart(char *read){
    //Define the current position in the buffer
    int pos = 0;
    //Read all that is present in the buffer
    while(1){
        //Pull up the maximum size of the buffer - starting at the current pos
        int n = uart_read_bytes(UART_PORT, read + pos, RX_BUFFER, pdMS_TO_TICKS(50));
        //Check that the last character is ! (that the message was read whole)
        //Since only one messages always pass we don't need to worry that multiple messages may be present in the buffer
        //If something was read
        if (n > 0){
            //Move teh current position
            pos += n;
            //Check what the last char is
            char last_char = read[pos-1];
            //If the last char is '!' - whole message read
            if (last_char == '!'){
                //Mesage correctly received
                return true;
            }
            //If the pos is now bigger than max buffer
            if (pos >= RX_BUFFER){
                //Buffer overflow - end the program
                return false;
            }
        }
    }
}