#include "avr.h"
#include "spi.h"
#include "datamanager.h"
#include "engine.h"
#include "avr/wdt.h"

// SPI spi(2 /* send */, 1 /* recv */);
DataManager manager(nullptr, 0);

ISR(SPI_STC_vect){
//	const byte* received = spi.byteFinished();
//	if (received) // transmission finished
//		manager.receivedValues(received);
}

int main(void)
{
	wdt_enable(WDTO_120MS);
	bitSet(PORTB, 0); // ???

	sei();
	while(true)
	{
		wdt_reset();
		manager.process();
	}
}
