#include "adc.h"

#include <math.h>

void InitADC(void)
{
    // AVCC reference
    ADMUX = (1 << REFS0);

    // Enable ADC, Prescaler = 128
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t ReadADC(uint8_t channel)
{
    channel &= 0x0F;

    ADMUX = (ADMUX & 0xF0) | channel;

    ADCSRA |= (1 << ADSC);

    while (ADCSRA & (1 << ADSC))
        ;

    return ADC;
}

uint8_t ADCToPercent(uint16_t adc_value)
{
    return (adc_value * 100UL) / 1023UL;
}

uint16_t ADCToFrequency(uint16_t adc)
{
    const uint16_t min_freq = 220;
    const uint16_t octaves = 2;

    float position = adc / 1023.0f;
    float freq = min_freq * powf(2.0f, position * octaves);

    return (uint16_t)(freq + 0.5f);
}
