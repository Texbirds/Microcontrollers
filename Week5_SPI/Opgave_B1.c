#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>

#define BIT(x)       ( 1<<x )
#define DDR_SPI      DDRB                    // spi Data direction register
#define PORT_SPI     PORTB                   // spi Output register
#define SPI_SCK      1                       // PB1: spi Pin System Clock
#define SPI_MOSI     2                       // PB2: spi Pin MOSI
#define SPI_MISO     3                       // PB3: spi Pin MISO
#define SPI_SS       0                       // PB0: spi Pin Slave Select

// wait(): busy waiting for 'ms' millisecond
// used library: util/delay.h
void wait(int ms)
{
    for (int i = 0; i < ms; i++)
    {
        _delay_ms(1);
    }
}

void spi_masterInit(void)
{
    DDR_SPI = 0xFF;                          // All pins output: MOSI, SCK, SS, SS_display as output
    DDR_SPI &= ~BIT(SPI_MISO);               // except: MISO input
    PORT_SPI |= BIT(SPI_SS);                 // SS_ADC == 1: deselect slave
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1);  // Enable SPI, MasterMode, Clock rate fck/64, bitrate=125kHz
    // Mode = 0: CPOL=0, CPPH=0;
}

// Write a byte from master to slave
void spi_write(unsigned char data)
{
    SPDR = data;                             // Load byte to Data register --> starts transmission
    while (!(SPSR & BIT(SPIF)));             // Wait for transmission complete
}

// Select device on pin number PORTB
void spi_slaveSelect(unsigned char chipNumber)
{
    PORTB &= ~BIT(chipNumber);
}

// Deselect device on pin number PORTB
void spi_slaveDeSelect(unsigned char chipNumber)
{
    PORTB |= BIT(chipNumber);
}

void spi_writeWord(unsigned char address, unsigned char data)
{
    spi_slaveSelect(0);     // Select display chip (MAX7219)
    spi_write(address);     // Write address byte to the display 
    spi_write(data);        // Write data byte to the selected address
    spi_slaveDeSelect(0);   // Deselect display chip
}

// Initialize the driver chip (MAX7219)
void displayDriverInit()
{
    spi_writeWord(0x09, 0xFF);     // Set decode mode for all digits to BCD mode
    spi_writeWord(0x0A, 0x0F);     // Set intensity to maximum (0x0F)
    spi_writeWord(0x0B, 0x03);     // Set scan limit to 4 digits (0x03)
    spi_writeWord(0x0C, 0x01);     // Set to normal operation (not shutdown)
}

// Main function
int main()
{
    // Initialize
    DDRB = 0x01;                     // Set PB0 pin as output for display select
    spi_masterInit();                 // Initialize SPI module
    displayDriverInit();              // Initialize display chip

    // Clear display (all zeros)
    for (char i = 1; i <= 4; i++)    // Loop over all 4 digits
    {
        spi_writeWord(i, 0);          // Write digit address i and digit value 0
    }
    wait(1000);                       // Wait for 1 second

    // Write 4-digit data (1-2-3-4)
    for (char i = 1; i <= 4; i++)    // Loop for all 4 digits
    {
        spi_writeWord(i, i);          // Write digit address i and digit value i (1, 2, 3, or 4)
        wait(1000);                   // Wait for 1 second
    }

    return (1);
}