/*
 * Implementation of the Led Controller
 *
 * Implements the functions for the integrated LED on the ESP32S3-WROOM-1
 */

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////
#include "led_controller.h"

///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////
static led_strip_handle_t led_strip;
static uint8_t s_led_state = 0;

///////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
///////////////////////////////////////////////////////////////////////////////
static void led_init(void)
{
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = 1,
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    /* Set all LED off to clear all pixels */
    led_strip_clear(led_strip);
}

static void led_blink(int frequency)
{
    /* do not turn the led off when frequency is o*/
    if (frequency == 0)
    {   /*white*/
        led_strip_set_pixel(led_strip, 0, 255, 255, 255);
        led_strip_refresh(led_strip);
        s_led_state = 1;
    }

    else
    {

        if (s_led_state)
        {
            /* changes colour depending on the frequency*/
            switch (frequency)
            {
            case 1000: /*red*/
                led_strip_set_pixel(led_strip, 0, 255, 0, 0);
                break;
            case 500: /*lime*/
                led_strip_set_pixel(led_strip, 0, 0, 255, 0);
                break;
            case 333:/*blue*/
                led_strip_set_pixel(led_strip, 0, 0, 0, 255);
                break;
            case 250:/*yellow*/
                led_strip_set_pixel(led_strip, 0, 255, 255, 0);
                break;
            case 200:/*Magenta*/
                led_strip_set_pixel(led_strip, 0, 255, 0, 255);
                break;
            case 166:/*Cyan*/
                led_strip_set_pixel(led_strip, 0, 0, 255, 255);
                break;
            case 142:/*orange*/
                led_strip_set_pixel(led_strip, 0, 255, 165, 0);
                break;
            case 125:/*dark green*/
                led_strip_set_pixel(led_strip, 0, 0, 100, 0);
                break;
            case 111:/*dark blue*/
                led_strip_set_pixel(led_strip, 0, 0, 0, 139);
                break;
            case 100:/*seashell*/
                led_strip_set_pixel(led_strip, 0, 255, 245, 238);
                break;
            default:/*deep pink*/
                led_strip_set_pixel(led_strip, 0, 255, 20, 147);
                break;
            }
            /* Refresh the strip to send data */
            led_strip_refresh(led_strip);
        }
        else
        {
            /* Set all LED off to clear all pixels */
            led_strip_clear(led_strip);
        }
        /* Toggle the LED state */
        s_led_state = !s_led_state;
    }
}
