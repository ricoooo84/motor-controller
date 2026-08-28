#ifndef INC_ADC_DMA_DRIVER_H_
#define INC_ADC_DMA_DRIVER_H_

#include <stdint.h>
#include "stm32f4xx.h"

#define ADC_CHANNEL	9
#define PWM_TRGO	8

extern volatile uint16_t adc_buf;

void adc_start(void);
void adc_amps(void);

#endif
