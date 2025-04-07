#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BIT(x) (1 << (x))

// Wachtfunctie
void wait(int ms)
{
	for (int tms = 0; tms < ms; tms++)
	{
		_delay_ms(1);
	}
}

// ADC initialisatie (10-bit, left adjusted, interne 2.56V referentie, ADC2 op PF2)
void adcInit(void)
{
	ADMUX = 0b11100010;  // AREF = 2.56V, left adjusted, kanaal 2 (PF2)
	ADCSRA = 0b10000110; // ADC aan, geen auto trigger, geen interrupt, prescaler 64
}

// Start ADC meting en retourneer de temperatuur (8-bit)
uint8_t adcRead(void)
{
	ADCSRA |= BIT(6);          // Start ADC conversie
	while (ADCSRA & BIT(6));   // Wachten tot conversie klaar is
	return ADCH;               // Retourneer de temperatuurwaarde (°C)
}

// --- LCD Functies ---
void lcd_strobe_lcd_e(void)
{
	PORTA |= (1 << 6);
	_delay_ms(1);
	PORTA &= ~(1 << 6);
	_delay_ms(1);
}

void lcd_command(unsigned char cmd)
{
	PORTC = (cmd & 0xF0);
	PORTA &= ~(1 << 4);
	lcd_strobe_lcd_e();
	PORTC = (cmd & 0x0F) << 4;
	PORTA &= ~(1 << 4);
	lcd_strobe_lcd_e();
}

void lcd_data(unsigned char data)
{
	PORTC = (data & 0xF0);
	PORTA |= (1 << 4);
	lcd_strobe_lcd_e();
	PORTC = (data & 0x0F) << 4;
	PORTA |= (1 << 4);
	lcd_strobe_lcd_e();
}

void lcd_init(void)
{
	DDRC |= 0xF0;
	DDRA |= 0x50;
	_delay_ms(15);
	lcd_command(0x28);
	lcd_command(0x0C);
	lcd_command(0x06);
	lcd_command(0x01);
}

void lcd_clear(void)
{
	lcd_command(0x01);
}

void lcd_set_line1(void)
{
	lcd_command(0x80);
}

void lcd_set_line2(void)
{
	lcd_command(0xC0);
}

void display_text(char *str)
{
	for (int i = 0; i < strlen(str); i++)
	{
		lcd_data(str[i]);
	}
}

void lcd_write_integer(uint16_t num)
{
	char buffer[6]; // Max 5 cijfers + null-terminator
	itoa(num, buffer, 10);
	display_text(buffer);
}

// --- LEDs Updaten ---
void updateLEDs(uint8_t temperature)
{
	PORTA = temperature; // Toon temperatuurwaarde binair op PORTA LEDs
}

// --- LCD Updaten ---
void updateLCD(uint8_t temperature)
{
	lcd_set_line2();
	lcd_write_integer(temperature);
	display_text(" C");
}

// --- Hoofdprogramma ---
int main(void)
{
	// Initialiseer hardware
	DDRF &= ~BIT(2); // PF2 (ADC2) als input voor LM35
	DDRA = 0xFF;     // PORTA als output voor LEDs
	DDRC |= 0xF0;    // PC4-7 als output (LCD)
	DDRA |= 0x50;    // PA4 en PA6 als output (LCD)
	
	lcd_init();
	lcd_clear();
	lcd_set_line1();
	display_text("Temp:"); // Eerste regel LCD

	adcInit(); // Initialiseer ADC

	while (1)
	{
		uint8_t temperature = adcRead(); // Lees temperatuurwaarde in °C
		updateLEDs(temperature);  // Update LEDs met temperatuurwaarde
		updateLCD(temperature);   // Update LCD met temperatuurwaarde
		wait(1000);               // Wacht 1 seconde
	}
}
