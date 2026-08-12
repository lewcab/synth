#ifndef ADC_H
#define ADC_H

#include <avr/io.h>
#include <stdint.h>

void InitADC(void);
uint16_t ReadADC(uint8_t channel);
uint8_t ADCToPercent(uint16_t adc_value);
uint16_t ADCToFrequency(uint16_t adc);

#endif
