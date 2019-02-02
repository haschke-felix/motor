#include "avr.h"
#include "avr/wdt.h"
#include "datamanager.h"
#include "engine.h"
#include "spi.h"
#include "usb.h"

 FUSES = {
    LFUSE_DEFAULT | (byte)~FUSE_CKDIV8, // run at 8MHz
    HFUSE_DEFAULT,                          // protect EEPROM from erase
   EFUSE_DEFAULT,
};
// SPI spi(2 /* send */, 1 /* recv */);
DataManager manager(500);

// ISR(SPI_STC_vect)
//{
//	const byte *received = spi.byteFinished();
//	if (received) // transmission finished

//		manager.receivedValues(received);
void foo(){

}

int main(void)
{
	wdt_enable(WDTO_120MS); // watchdog enable
	bitSet(PORTB, 0);       // led set high
	bitSet(PORTD,4);
	USB::USBinit(&foo, &foo);
	sei();
	USB::println("Startup at 9600 Baud");
	PORTC = 0;
	DDRC = 0;
	while (true)
	{
		wdt_reset(); // watchdog reset
		manager.process();
//		bitClear(PORTC,7);
//		bitClear(DDRC,7);
//		USB::println("hi");
	}
}
