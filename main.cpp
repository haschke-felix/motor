#include"avr.h"
#include"spi.h"
#include"datamanager.h"
#include "engine.h"



SPI spi(16);
DataManager manager(&spi, 0); // with sampling rate = 0, nether a transmission would be occur

ISR(SPI_STC_vect){
	if(spi.byteFinished()){ // transmission finished
		manager.transmissionFinished();
	}
}

int main(void)
{
#if 0
	TCCR1A|=(1<<WGM10);
	TCCR1A |= (1 << COM1A1);
	TCCR1B |= (1 << CS10)|(1 << CS10);
	OCR1A = 0xFF; // out at OC1A

	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	TCCR0B |= (1 << CS01);
	OCR0B = 0xFF;
	OCR0A = 0xFF;
	sei();
	bitSet(DDRB,1);
	OCR1A = 50;

#endif
	while(true)
	{
		manager.process();

//		bitSet(PORTB,1);
	}
}

