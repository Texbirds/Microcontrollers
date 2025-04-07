#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT(x)	(1 << (x))

// wait(): busy waiting for 'ms' milliseconds
void wait(int ms)
{
	for (int tms = 0; tms < ms; tms++)
	{
		_delay_ms(1); // Library function (max 30 ms at 8MHz)
	}
}

// Initialize ADC: 10-bits (left justified), single conversion mode, channel 2 (PF2)
void adcInit(void)
{
	ADMUX = 0b01100010;  // AREF=VCC, left adjusted, select ADC2 (PF2)
	ADCSRA = 0b10000110; // ADC enable, no auto trigger, no interrupt, division by 64
}

// Start ADC conversion and return result
void adcStartConversion(void)
{
	ADCSRA |= BIT(6);          // Start ADC conversion
	while (ADCSRA & BIT(6));   // Wait until conversion is complete
}

int main(void)
{
	DDRF &= ~BIT(2); // Set PF2 as input (ADC2)
	DDRA = 0xFF;     // Set PORTA for output (LEDs)
	DDRB = 0xFF;     // Set PORTB for output (LEDs)
	adcInit();       // Initialize ADC

	while (1)
	{
		adcStartConversion();  // Start ADC conversion on request
		PORTB = ADCL;          // Show 2 LSB of ADC value on PORTB
		PORTA = ADCH;          // Show 8 MSB of ADC value on PORTA
		wait(1000);             // Every 100 ms (busy waiting)
	}
}
