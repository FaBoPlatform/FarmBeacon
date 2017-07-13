#ifndef ADC_H___
#define ADC_H___

#include "nrf_drv_saadc.h"

void adc_init(nrf_saadc_input_t pin);
int16_t adc_check(void);

#endif // ADC_H___
