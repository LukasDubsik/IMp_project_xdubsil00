/* INCLUDES */
//User defined Includes
#include "led.h"
#include "../imp.h"

//System headers
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "soc/gpio_num.h"

void configure_led(void)
{
    gpio_reset_pin(GPIO_NUM_2);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
}

void set_led(bool state)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(GPIO_NUM_2, state);
}

void blink_led(int on_time)
{
    //Turn the led on
    set_led(HIGH);
    //Wait for the given time - ms converted to cycles
    vTaskDelay(pdMS_TO_TICKS(on_time));
    //Turn the led off
    set_led(LOW);
}

void blink_error(int error_type)
{
    for (int i=0; i < error_type; i++){
        blink_led(on_time);
    }
}