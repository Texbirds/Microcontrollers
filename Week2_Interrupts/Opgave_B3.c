#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>

// Lookup-table voor 7-segment display (hexadecimaal + E)
// PORTD = dp g f e d c b a
const unsigned char segmentLUT[17] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // A
    0x7C, // b
    0x39, // C
    0x5E, // d
    0x79, // E
    0x71, // F
    0x79  // E (Error)
};

// Delay functie
void wait(int ms) {
    for (int i = 0; i < ms; i++) {
        _delay_ms(1);
    }
}

// Display functie: toont hexadecimale waarde op 7-segment
void display(int digit) {
    if (digit >= 0 && digit <= 15) {
        PORTD = segmentLUT[digit];
        } else {
        PORTD = segmentLUT[16]; // E voor error
    }
}

// Knop lezen: retourneert 1 als ingedrukt
int buttonPressed(uint8_t pin) {
    return !(PINA & (1 << pin));
}

int main(void) {
    // Initialisatie
    DDRD = 0xFF;    // PORTD: output (7-segment)
    DDRA = 0x00;    // PORTA: input (knoppen)
    PORTA = 0xFF;   // Pull-up weerstanden voor PORTA

    int value = 0;

    while (1) {
        // Controleer knoppen
        if (buttonPressed(PINA0) && buttonPressed(PINA1)) {
            // Beide knoppen: reset naar 0
            value = 0;
            display(value);
            wait(300);
        }
        else if (buttonPressed(PINA0)) {
            // UP-knop: waarde verhogen (max. F)
            if (value < 15) value++;
            display(value);
            wait(300);
        }
        else if (buttonPressed(PINA1)) {
            // DOWN-knop: waarde verlagen (min. 0)
            if (value > 0) value--;
            display(value);
            wait(300);
        }

        // Toon huidige waarde
        display(value);
        wait(50);
    }
    
    return 0;
}
