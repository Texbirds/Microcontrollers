void writeLedDisplay(int value) {
	char digits[4];  // Array voor 4 cijfers
	int tempValue = value;
	int isNegative = 0;

	// Controleer of de waarde negatief is
	if (value < 0) {
		isNegative = 1;
		tempValue = -value;  // Maak de waarde positief voor verwerking
	}

	// Splits de waarde op in afzonderlijke cijfers van rechts naar links
	for (int i = 3; i >= 0; i--) {
		digits[i] = tempValue % 10;  // Haal het laatste cijfer eruit
		tempValue /= 10;             // Verwijder het laatste cijfer
	}

	// Als het negatief is, zet dan een '-' teken op de eerste digit (digit 1)
	if (isNegative) {
		digits[0] = 10;  // MAX7219 heeft geen minteken, we gebruiken '10' voor een minus teken
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
