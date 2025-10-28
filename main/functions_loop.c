#include "imp.h"
#include "periferies/matrix.h"
#include "periferies/led.h"

#include "esp_log.h"

bool scan_for_selection(char *valid, char *returned){
    *returned = wait_for_press(valid);
    //Logging to test if all is received correctly - for debugging here
    ESP_LOGI(TAG, "Pressed the key: %c!", *returned);
    //If we have gotten the RESTART back
    if (*returned == RESTART_KEY){
        //Meaning that restart was triggered
        blink_error(3);
        //Return false to inform about restart
        return false;
    }
    //Otherwise blink the LED to signify the input has been accepted
    blink_led(CORRECT_LED_TIME);
    //Return given and it wasn't a restart
    return true;
}