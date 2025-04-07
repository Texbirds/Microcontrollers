/*
 * lcd.c
 *
 * Created: 13-2-2025 21:47:47
 *  Author: hajlinde
 *
 * This code is specifically for the UNI-DS6 board:
 *  LCD data lines on PC4-PC7 (4 bit mode)
 *  LCD RS line on PA4
 *  LCD E line on PA6
 */ 

 #include <stddef.h>
 #include <avr/io.h>
 #define F_CPU 10000000UL
 #include <util/delay.h>
 #include <avr/interrupt.h>
 #include <string.h>
 
 typedef unsigned char byte;
 
 /* Custom character set */
 static byte charset[] =
 {
     0b00000000,
     0b00000100,
     0b00001110,
     0b00011111,
     0b00011111,
     0b00001110,
     0b00000100,
     0b00000000,
 
     0b00011111,
     0b00001110,
     0b00000100,
     0b00000000,
     0b00000000,
     0b00000100,
     0b00001110,
     0b00011111
 };
 
 void lcd_strobe_lcd_e(void)
 {
     PORTA |= (1<<6);
     _delay_ms(1);
     PORTA &= ~(1<<6);
     _delay_ms(1);
 }
 
 void lcd_cmd_high_nibble(byte cmd)
 {
     PORTC = (cmd & 0xF0); // High nibble to PC4-7
     PORTA &= ~(1<<4); // Force RS=PA4 low
 }
 
 void lcd_cmd_low_nibble(byte cmd)
 {
     PORTC = (cmd & 0x0F) << 4; // Low nibble to PC4-7
     PORTA &= ~(1<<4); // Force RS=PA4 low
 }
 
 void lcd_data_high_nibble(byte data)
 {
     PORTC = (data & 0xF0); // High nibble to PC4-7
     PORTA |= (1<<4); // Force RS=PA4 high
 }
 
 void lcd_data_low_nibble(byte data)
 {
     PORTC = (data & 0x0F) << 4; // Low nibble to PC4-7
     PORTA |= (1<<4); // Force RS=PA4 high
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
     // Note: this is for UNI-DS6 development board
     // Set appropriate pins to output
     PORTC &= 0x0F; // Set output pins low first
     PORTA &= 0x50;
     DDRC |= 0xF0; // Make PC4-7 output
     DDRA |= 0x50; // Make PA4 (RS) and PA6 (E) output
     
     // Switch from 8 bit to 4 bit mode
     // Note: first command is still in 8 bit mode
     PORTC = 0x20; // Function set: 0010 0000
     lcd_strobe_lcd_e();
     // Note: now in 4 bit mode, so send all data in nibbles
     
     // Set 2 lines, 5x8 dots characters
     lcd_command(0x28); // Function set: 0010 1000
     
     // Set display on, cursor off, blinking off
     lcd_command(0x0C); // Display on/off control: 0000 1100
     
     // Set cursor increment, no shift
     lcd_command(0x06); // Entry mode set: 0000 0110
     
     // Clear display, set data address 0 (top left)
     lcd_command(0x01); // Clear display
 }
 
 void lcd_clear(void)
 {
     lcd_command(0x01);
 }
 
 void lcd_return_home(void)
 {
     lcd_command(0x02);
 }
 
 void lcd_fill_custom_chars(void)
 {
     lcd_command(0x40); // Set CGRAM address to 0
     for (int ch = 0; ch < sizeof charset; ch++)
     {
         lcd_data(charset[ch]);
     }
     lcd_return_home();
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
 
 int main(void)
 {
     lcd_init_4bits_mode();
     lcd_fill_custom_chars();
     lcd_clear();
     
     lcd_set_line1();
     display_text("Kwint");
     _delay_ms(800);
     
     lcd_set_line2();
     display_text("Pieter");
     _delay_ms(900);
     
     lcd_clear();
     display_text("Beste team! :)");
     _delay_ms(1200);
     
     while (1)
     {
         // Show two custom characters
         //lcd_data_address(0x4F);
         //lcd_data(0x00);
         //_delay_ms(500);
         //lcd_data_address(0x4F);
         //lcd_data(0x01);
         //_delay_ms(500);
         
         // Show entire character set
         // for (byte b = 0; b < 256; b++)
         // {
             // lcd_data_address(0x4F);
             // lcd_data(b);
             // _delay_ms(300);
         // }
     }
     return 0;
 }
