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
    // Set all pins of PORTA & PORTD as outputs
    DDRA = 0xFF; 
    DDRD = 0xFF; 
    
    while (1) 
    {
        // Turn on the LED connected to PD6 (bit 6)
        PORTD = 0b01000000;
        wait(500); // Wait for 500 ms
        
        // Turn off PD6 and turn on the LED connected to PD7 (bit 7)
        PORTD = 0b10000000;
        wait(500); // Wait for 500 ms
    }
}
