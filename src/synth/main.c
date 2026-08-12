#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>

#include "utils/adc.h"
#include "utils/lcd.h"

//------------------------------------------------------
// Convert percentage to LED bar graph
//------------------------------------------------------
uint8_t PercentToLED(uint8_t percent)
{
	if (percent < 12)
		return 0b00000001;
	if (percent < 25)
		return 0b00000011;
	if (percent < 37)
		return 0b00000111;
	if (percent < 50)
		return 0b00001111;
	if (percent < 62)
		return 0b00011111;
	if (percent < 75)
		return 0b00111111;
	if (percent < 87)
		return 0b01111111;

	return 0b11111111;
}

//------------------------------------------------------
// Initialize Timer1 to generate square wave on PB5
// (Arduino Digital Pin 11)
//------------------------------------------------------
void InitTone(void)
{
	DDRB |= (1 << PB5); // PB5 output (OC1A)

	TCCR1A = (1 << COM1A0); // Toggle OC1A on compare match
	TCCR1B = (1 << WGM12);	// CTC mode
}

//------------------------------------------------------
// Set tone frequency
//------------------------------------------------------
void SetTone(uint16_t frequency)
{
	if (frequency == 0)
	{
		TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); // Stop timer
		PORTB &= ~(1 << PB5);
		return;
	}

	OCR1A = (F_CPU / (2UL * 8UL * frequency)) - 1;

	TCCR1B = (1 << WGM12) | (1 << CS11); // Prescaler = 8
}

//------------------------------------------------------
// Clear Tone
//------------------------------------------------------
void ClearTone()
{
	PORTC = 0x00;
	SetTone(0);
}

//------------------------------------------------------
// Main
//------------------------------------------------------
int main(void)
{
	uint16_t curr_adc;
	uint8_t lcd_counter = 0;

	// LEDs on PORTC (Arduino pins 30-37)
	DDRC = 0xFF;

	InitADC();
	InitTone();
	InitLCD(LS_BLINK);

	while (1)
	{
		curr_adc = ReadADC(0);

		if (curr_adc > 5)
			ClearTone();
		else
		{
			uint8_t percent = ADCToPercent(curr_adc);
			PORTC = PercentToLED(percent);

			uint16_t frequency = ADCToFrequency(curr_adc);
			SetTone(frequency);
		}

		if (lcd_counter > 100)
		{
			LCDClear();
			LCDWriteStringXY(0, 0, "ADC Value:");
			LCDWriteIntXY(0, 1, curr_adc, 4);
			lcd_counter = 0;
		}
		else
		{
			lcd_counter++;
		}

		_delay_ms(20);
	}
}