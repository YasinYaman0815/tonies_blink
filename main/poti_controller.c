/*
 * Implementation of the Poti
 *
 * Implements the functions for the potentiometer
 */

///////////////////////////////////////////////////////////////////////////////
//Includes
///////////////////////////////////////////////////////////////////////////////
#include "poti_controller.h"

///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////
static esp_adc_cal_characteristics_t adc1_chars;
static int Frequency = 0;

///////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
///////////////////////////////////////////////////////////////////////////////

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
