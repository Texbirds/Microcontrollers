/*
 *
 * Created: 17-2-2025
 * Author : Kwint
 */

 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <util/delay.h>
 
 #define F_CPU 8e6  // Define CPU frequency for timing functions
 
 // Global variable to track the LED position
 volatile uint8_t led_position = 0x01;
 
 void init_ports() {
     // Set PORTC as output to control the LEDs
     DDRC = 0xFF;  // All pins on PORTC as output
     PORTC = led_position; // Initialize LEDs with first LED on
 
     // Configure INT1 and INT2 for falling edge triggers
     EICRA |= (1 << ISC11); // INT1: Falling edge triggers interrupt (PD3)
     EICRB |= (1 << ISC21); // INT2: Falling edge triggers interrupt (PD2)
 
     // Enable INT1 and INT2 interrupts
     EIMSK |= (1 << INT1) | (1 << INT2);
 
     // Enable global interrupts
     sei(); // Set the global interrupt enable bit
 }

 ISR(INT1_vect) {
     // Shift LED left: leftmost bit rotates to the rightmost position
     led_position = (led_position << 1) | (led_position >> 7);
     PORTC = led_position; // Update LEDs
     _delay_ms(300); // Debounce delay
 }
 


 ISR(INT2_vect) {
     // Shift LED right: rightmost bit rotates to the leftmost position
     led_position = (led_position >> 1) | (led_position << 7);
     PORTC = led_position; // Update LEDs
     _delay_ms(300); // Debounce delay
 }
 
 int main(void) {
     // Initialize ports and configure interrupts
     init_ports();
 
     // Main loop remains idle while interrupts handle LED shifting
     while (1) {
         // Do nothing - interrupts handle LED shifts
     }
 
     return 0;
 }
