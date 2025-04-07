/*
 *
 * Created: 7-2-2025 10:41:33
 * Author : Kwint
 */

 #define F_CPU 10000000UL  // Define CPU frequency for delay calculations

 #include <avr/io.h>
 #include <util/delay.h>
 
 // Global variable for LED refresh rate (in ms)
 int refreshRate = 1000;
 
 // Function to create a delay in milliseconds
 void wait(int ms) {
     for (int i = 0; i < ms; i++) {
         _delay_ms(1); // Delay of 1 ms per iteration
     }
 }
 
 // Function to check button state and toggle the refresh rate
 void checkButton() {
     // Check if button connected to PC0 is pressed
     if (PINC & 0x01) {
         // Toggle refresh rate between 1000 ms and 250 ms
         if (refreshRate == 1000) {
             refreshRate = 250;
         } else {
             refreshRate = 1000;
         }
 
         // Simple debounce: wait for button release
         wait(300);
     }
 }
 
 int main(void)
 {
     // Configure PORTC as input (for button) and PORTD as output (for LED)
     DDRC = 0x00; // Set PORTC as input
     DDRD = 0xFF; // Set PORTD as output
     
     while (1) 
     {
         // Turn LED on
         PORTD = 0b10000000; // Turn on LED connected to PD7
         wait(refreshRate);  // Wait for the current refresh interval
         checkButton();      // Check if button was pressed
 
         // Turn LED off
         PORTD = 0b00000000; // Turn off LED
         wait(refreshRate);  // Wait for the current refresh interval
         checkButton();      // Check if button was pressed
     }
 }
