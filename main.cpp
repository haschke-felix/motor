#include "avr.h"
#include "spi.h"
#include "datamanager.h"
#include "engine.h"

SPI spi(16);
DataManager manager(&spi, 0xFF);

ISR(SPI_STC_vect){
	if(spi.byteFinished()){ // transmission finished
		manager.transmissionFinished();
	}
}

int main(void)
{
	bitSet(PORTB,0);
	while(true)
	{
		manager.process();
	}
}

