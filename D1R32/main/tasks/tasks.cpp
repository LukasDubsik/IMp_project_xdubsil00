#include "tasks.h"

// Includes
#include "../imp.h"
#include "../periferies/matrix.h"

// System Includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void scan_restart(void *arg)
{
    // Forever scan for possible interrupts
    while (1) {
        char res = scan_keyboard();

        if (res != '\0') {
            if (res == RESTART_KEY) {
                restart_pressed = true;
            }
        }

        // Give a breathing room
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}