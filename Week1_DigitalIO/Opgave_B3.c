/*
 *
 * Created: 7-2-2025 10:41:33
 * Author : Kwint
 */ 

// Define CPU frequency for delay functions
#define F_CPU 10000000UL

#include <avr/io.h>
#include <util/delay.h>

// Function to create a delay in milliseconds
void wait(int ms) {
    for (int i = 0; i < ms; i++) {
        _delay_ms(1); // Delay of 1 ms per iteration
    }
}

int main(void)
{
    // Configure PORTC as input (for button)
    DDRC = 0x00; // All pins of PORTC are inputs
    // Configure PORTD as output (for LEDs)
    DDRD = 0xFF; // All pins of PORTD are outputs
    
    while (1) 
    {
        // Check if the button connected to PC0 is pressed
        if (PINC & 0x01) // If bit 0 of PINC is high (button pressed)
        {
            // Blink the LED connected to PD7
            PORTD = 0b10000000; // Turn on LED on PD7
            wait(500);          // Wait for 500 ms
            
            PORTD = 0b00000000; // Turn off LED
            wait(500);          // Wait for 500 ms
        }
        else
        {
            // Keep LEDs off if button isn't pressed
            PORTD = 0b00000000;
        }
    }
}
