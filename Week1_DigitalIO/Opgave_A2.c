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
	    // Set all pins of PORTA as output
	    DDRA = 0xFF; // DATA direction register A
	    
	    // Main program loop
	    while (1) 
	    {
	        // Turn on the upper 4 LEDs and turn off the lower 4 LEDs
	        PORTA = 0b11110000;
	        wait(250); // Wait for 250 ms
	        
	        // Turn on the lower 4 LEDs and turn off the upper 4 LEDs
	        PORTA = 0b00001111;
	        wait(250); // Wait for 250 ms
	    }
	}
