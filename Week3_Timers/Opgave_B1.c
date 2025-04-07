/*
 * lcd_counter.c
 *
 * Created: 13-2-2025 21:47:47
 * Author: hajlinde
 *
 * Functie: Toont op het LCD hoe vaak een knop op PA0 is ingedrukt.
 */

 #include <stddef.h>
 #include <avr/io.h>
 #define F_CPU 10000000UL
 #include <util/delay.h>
 #include <avr/interrupt.h>
 #include <string.h>
 #include <stdio.h>
 #include <stdlib.h>
 
 typedef unsigned char byte;
 
 volatile uint16_t press_count = 0;  // Variabele voor de teller
 
 typedef unsigned char byte;
 
 volatile uint8_t last_button_state = 1;
 
 void lcd_write_integer(uint16_t num);
 void display_text(char *str);
 void lcd_set_line2(void);
 
 void lcd_strobe_lcd_e(void)
 {
     PORTA |= (1<<6);
     _delay_ms(1);
     PORTA &= ~(1<<6);
     _delay_ms(1);
 }
 
 void lcd_cmd_high_nibble(byte cmd)
 {
     PORTC = (cmd & 0xF0); // High nibble naar PC4-7
     PORTA &= ~(1<<4); // RS=PA4 laag (commando)
 }
 
 void lcd_cmd_low_nibble(byte cmd)
 {
     PORTC = (cmd & 0x0F) << 4; // Low nibble naar PC4-7
     PORTA &= ~(1<<4); // RS=PA4 laag (commando)
 }
 
 void lcd_data_high_nibble(byte data)
 {
     PORTC = (data & 0xF0); // High nibble naar PC4-7
     PORTA |= (1<<4); // RS=PA4 hoog (data)
 }
 
 void lcd_data_low_nibble(byte data)
 {
     PORTC = (data & 0x0F) << 4; // Low nibble naar PC4-7
     PORTA |= (1<<4); // RS=PA4 hoog (data)
 }
 
 void lcd_command(byte cmd)
 {
     lcd_cmd_high_nibble(cmd);
     lcd_strobe_lcd_e();
     lcd_cmd_low_nibble(cmd);
     lcd_strobe_lcd_e();
 }
 
 void lcd_data(byte data)
 {
     lcd_data_high_nibble(data);
     lcd_strobe_lcd_e();
     lcd_data_low_nibble(data);
     lcd_strobe_lcd_e();
 }
 
 void lcd_init_4bits_mode(void)
 {
     PORTC &= 0x0F;
     PORTA &= 0x50;
     DDRC |= 0xF0;
     DDRA |= 0x50;
 
     PORTC = 0x20;
     lcd_strobe_lcd_e();
 
     lcd_command(0x28);
     lcd_command(0x0C);
     lcd_command(0x06);
     lcd_command(0x01);
 }
 
 void lcd_clear(void)
 {
     lcd_command(0x01);
 }
 
 void lcd_return_home(void)
 {
     lcd_command(0x02);
 }
 
 void lcd_data_address(byte address)
 {
     lcd_command(address | 0x80);
 }
 
 void lcd_set_line1(void)
 {
     lcd_data_address(0x00);
 }
 
 void lcd_set_line2(void)
 {
     lcd_data_address(0x40);
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
 
 /* --- Timer2 instellen als teller op PA0 --- */
 void timer2_init(void)
 {
     DDRB &= ~(1 << PB2); // PB2 (T2) als input (knop)
     PORTB |= (1 << PB2); // Pull-up aanzetten
 
     TCCR2 = (1 << CS22) | (1 << CS21) | (1 << CS20); // Externe klok (T2, stijgende flank)
     TIMSK |= (1 << TOIE2);
     sei(); 
 }
 
 /* --- ISR voor Timer2 Overflow --- */
 ISR(TIMER2_OVF_vect)
 {
     press_count += 256; // Tel door na overflow
 }
 
 void check_button_press(void)
 {
     uint8_t current_state = PINA & (1 << PA0);
     
     if (current_state == 0 && last_button_state == 1)
     {
         _delay_ms(50);
         if ((PINA & (1 << PA0)) == 0)
         {
             press_count++; // Increment count by exactly 1
         }
     }
     
     last_button_state = current_state;
 }
 
 void button_init(void)
 {
     DDRA &= ~(1 << PA0); // PA0 as input (button)
     PORTA |= (1 << PA0); // Enable pull-up resistor (button is active LOW)
 }
 
 /* --- Hoofdprogramma --- */
 int main(void)
 {
     lcd_init_4bits_mode();
     lcd_clear();
     lcd_set_line1();
     display_text("Press Count:");
     
     button_init();
     
     while (1)
     {
         check_button_press();
         lcd_set_line2();
         lcd_write_integer(press_count);
         _delay_ms(100);
     }
 }
 