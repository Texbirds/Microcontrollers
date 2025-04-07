#define F_CPU 8e6
#include <avr/io.h>
#include <avr/interrupt.h>

#define BIT(x) (1 << (x))

volatile uint8_t state = 0;

void timer2Init(void) {
	TCCR2 = (1 << WGM21) | (1 << CS22) | (1 << CS20);
	OCR2 = 1875;
	TIMSK |= (1 << OCIE2);
}

ISR(TIMER2_COMP_vect) {
	if (state == 0) {
		PORTD |= BIT(7); // Zet PD7 hoog
		OCR2 = 1875; // 15 ms hoog
		state = 1;
		} else {
		PORTD &= ~BIT(7); // Zet PD7 laag
		OCR2 = 3125; // 25 ms laag
		state = 0;
	}
}

int main(void) {
	DDRD |= BIT(7); // Stel PD7 in als output
	timer2Init();
	sei();
	
	while (1) {
	}
}
