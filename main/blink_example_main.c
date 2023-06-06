#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

static const char *TAG = "example";

#define BLINK_GPIO CONFIG_BLINK_GPIO

static uint8_t s_led_state = 0;

static led_strip_handle_t led_strip;

#include "driver/adc.h"
#include "esp_adc_cal.h"
static esp_adc_cal_characteristics_t adc1_chars;
static int Frequency = 0;

static void poti_init(void)
{
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    adc1_config_width(ADC_WIDTH_BIT_DEFAULT);
    adc1_config_channel_atten(ADC1_CHANNEL_2, ADC_ATTEN_DB_11);
}

static int poti_read(void)
{
    int voltageInput = esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_2), &adc1_chars);
    int step = voltageInput / 90;

    if (step == 0)
    {
        Frequency = 0;
    }
    else
    {
        Frequency = 1000 / step;
    }
    return Frequency;
}

static void blink_led(int frequency)
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

static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink addressable LED!");
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = 1, // at least one LED on board
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    /* Set all LED off to clear all pixels */
    led_strip_clear(led_strip);
}

void app_main(void)
{
    configure_led();
    poti_init();
    int Flast = 0;
    int FCountdown = 0;

    while (1)
    {
        int FInput = poti_read();

        if (FInput != Flast)
        {
            /*update Frequency*/
            FCountdown = FInput;
            Flast = FInput;
            blink_led(FCountdown);
        }
        else if (FCountdown <= 10 && Flast != 0)
        {
            /*Countdown expired, trigger blink*/
            FCountdown = Flast;
            blink_led(FCountdown);
        }
        else
        {
            /*reduce Countdown by 10ms*/
            vTaskDelay(10 / portTICK_PERIOD_MS);
            FCountdown = FCountdown - 10;
        }
    }
}
