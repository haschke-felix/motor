#include <avr.h>

void spi_init_slave (void)
{
	DDRB=(1<<PINB4);               // MISO as OUTPUT
	SPCR=(1<<SPE)|(1<<SPIE);       // Enable SPI && interrupt enable bit
	SPDR=0;
}


ISR(SPI_STC_vect)
{
	uint8_t data = SPDR;
	SPDR = ++data;
}

int main(void)
{
	spi_init_slave();

	sei();
	while(1)
	{}
}
