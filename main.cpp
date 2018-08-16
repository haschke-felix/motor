#include"avr.h"
#include"spi.h"
#include"datamanager.h"
#include "engine.h"



//   DataManager manager;
SPI spi(16);
DataManager manager(&spi, 0xFFFF);
//Engine engine;

ISR(SPI_STC_vect){
	if(spi.byteFinished()){ // transmission finished
		manager.transmissionFinished();
	}
}

int main(void)
{
#if 1

//	ADCSRA |= _BV(ADEN);
//	bitSet(ADMUX,REFS0);

//	bitSet(DDRB,1);
//	bitSet(DDRD,7);
//	bitClear(PORTB,1);
//	bitClear(PORTD,7);

	sei();

//	control.init();
#endif
//	bitSet(DDRC,4);
//	bitSet(PORTC,4);
//+
	while(true)
	{
		manager.process();
	}
}

