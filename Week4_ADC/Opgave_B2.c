#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT(x)	(1 << (x))

// wait(): busy waiting for 'ms' millisecond
void wait(int ms)
{
	for (int tms = 0; tms < ms; tms++)
	{
		_delay_ms(1); // library function (max 30 ms at 8MHz)
	}
}

// Initialize ADC: 10-bits (left justified), free running, channel 2 (PF2)
void adcInit(void)
{
	ADMUX = 0b01100010;  // AREF=VCC, left adjusted, select ADC2 (PF2)
	ADCSRA = 0b11100110; // ADC-enable, no interrupt, start, free running, division by 64
}

int main(void)
{
	DDRF &= ~BIT(2); // Set PF2 as input (ADC2)
	DDRA = 0xFF;     // Set PORTA for output (LEDs)
	DDRB = 0xFF;     // Set PORTB for output (LEDs)
	adcInit();       // Initialize ADC

	while (1)
	{
		PORTB = ADCL;  // Show 2 LSB of ADC value on PORTB
		PORTA = ADCH;  // Show 8 MSB of ADC value on PORTA
		wait(100);     // Every 100 ms (busy waiting)
	}
}
