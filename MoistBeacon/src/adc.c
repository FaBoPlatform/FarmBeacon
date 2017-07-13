#include "adc.h"

static void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
}

void adc_init(nrf_saadc_input_t pin)
{
    ret_code_t err_code;
    nrf_saadc_channel_config_t channel_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(pin);

    err_code = nrf_drv_saadc_init(NULL, saadc_callback);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_channel_init(0, &channel_config);
    APP_ERROR_CHECK(err_code);
}

int16_t adc_check(void)
{
    nrf_saadc_value_t adc_result;
    nrf_drv_saadc_sample_convert(0, &adc_result);
    return adc_result;
}