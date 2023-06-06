/*
 * Main Implementation for the Application task from Tonies
 */

// Includes
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "led_controller.c"
#include "poti_controller.c"

void app_main(void)
{
    /*initialize the LED and Potentiometer*/
    led_init();
    poti_init();

    /*Last saved Frequency*/
    int Flast = 0;

    /*Frequency countdown will be reduced by 10ms after each loop*/
    int FCountdown = 0;

    while (1)
    {

        /*Read out the current Frequency defined by the poti*/
        int FInput = poti_read();


        if (FInput != Flast)
        {
            /*update Frequency*/
            FCountdown = FInput;
            Flast = FInput;
            led_blink(FCountdown);
        }
        else if (FCountdown <= 10 && Flast != 0)
        {
            /*Countdown expired, trigger blink*/
            FCountdown = Flast;
            led_blink(FCountdown);
        }
        else
        {
            /*reduce Countdown by 10ms*/
            vTaskDelay(10 / portTICK_PERIOD_MS);
            FCountdown = FCountdown - 10;
        }
    }
}
