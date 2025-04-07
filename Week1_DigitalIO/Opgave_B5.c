#define F_CPU 10000000UL  // Define the CPU frequency for delay calculations

#include <avr/io.h>
#include <util/delay.h>

// Define a struct to represent LED patterns with associated delays
typedef struct {
    unsigned char data;  // LED pattern (bit pattern for PORTD)
    unsigned int delay;  // Delay in milliseconds
} PATTERN_STRUCT;

// Array of LED patterns with corresponding delays
PATTERN_STRUCT pattern[] = {
    // Single LEDs lighting up in sequence
    {0x00, 100}, {0x01, 100}, {0x02, 100}, {0x04, 100}, {0x10, 100}, {0x20, 100}, {0x40, 100}, {0x80, 100},

    // Pause
    {0x00, 100},

    // Alternating pattern (binary: 10101010 and 01010101)
    {0xAA, 50}, {0x55, 50},
    {0xAA, 50}, {0x55, 50},
    {0xAA, 50}, {0x55, 50},

    // Pause
    {0x00, 100},

    // Symmetrical pattern moving inward and outward
    {0x81, 100}, {0x42, 100}, {0x24, 100}, {0x18, 100},

    // Toggle between lower and upper halves
    {0x0F, 200}, {0xF0, 200}, {0x0F, 200}, {0xF0, 200},

    // End of pattern indicated by delay = 0
    {0x00, 0x00}
};

// Function to create a delay in milliseconds
void wait(int ms) {
    for (int i = 0; i < ms; i++) {
        _delay_ms(1); // Delay of 1 ms per iteration
    }
}

int main(void)
{
    // Set PORTD as output to display LED patterns
    DDRD = 0xFF; // Data direction register D: all pins as outputs

    // Main program loop
    while (1) 
    {
        int index = 0;

        // Iterate through the pattern array until delay = 0 (stop marker)
        while (pattern[index].delay != 0) {
            // Output the LED pattern to PORTD
            PORTD = pattern[index].data;

            // Wait for the specified duration
            wait(pattern[index].delay);

            // Move to the next pattern
            index++;
        }
    }
}
