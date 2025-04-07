void writeLedDisplay(int value) {
	// We gaan ervan uit dat 'value' maximaal 4 cijfers bevat
	char digits[4];  // Array voor 4 cijfers
	int tempValue = value;

	// Splits de waarde op in afzonderlijke cijfers van rechts naar links
	for (int i = 3; i >= 0; i--) {
		digits[i] = tempValue % 10;  // Haal het laatste cijfer eruit
		tempValue /= 10;             // Verwijder het laatste cijfer
	}

	// Nu de digits array gevuld is met de cijfers van de waarde,
	// sturen we ze naar het display.
	for (int i = 0; i < 4; i++) {
		spi_slaveSelect(0);               // Selecteer het display
		spi_write(i + 1);                 // Digit adres (1..4)
		spi_write(digits[i]);             // Waarde voor die digit
		spi_slaveDeSelect(0);             // Deselecteer het display
	}
}
