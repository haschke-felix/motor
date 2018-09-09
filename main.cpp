#include "avr.h"
#include "spi.h"
#include "datamanager.h"
#include "engine.h"
#include "avr/wdt.h"

SPI spi(16);
DataManager manager(nullptr, 0);

ISR(SPI_STC_vect){
	if(spi.byteFinished()){ // transmission finished
		manager.transmissionFinished();
	}
}

int main(void)
{
	wdt_enable(WDTO_120MS);
	bitSet(PORTB,0);
	while(true)
	{
		wdt_reset();
		manager.process();
	}
}

