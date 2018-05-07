#include"avr.h"
#include"spi.h"
#include"datamanager.h"
#include "engine.h"
//twi twi;

volatile byte send_buffer[32];
volatile byte receive_buffer[32];
volatile bool evaluate;





//   DataManager manager;
//DataManager manager();
//Engine engine();
Control control;


uint16_t adc_read(uint8_t adcx) {
	/* adcx is the analog pin we want to use.  ADMUX's first few bits are
	 * the binary representations of the numbers of the pins so we can
	 * just 'OR' the pin's number with ADMUX to select that pin.
	 * We first zero the four bits by setting ADMUX equal to its higher
	 * four bits. */
	ADMUX	&=	0xf0;
	ADMUX	|=	adcx;

	/* This starts the conversion. */
	ADCSRA |= _BV(ADSC);

	/* This is an idle loop that just wait around until the conversion
	 * is finished.  It constantly checks ADCSRA's ADSC bit, which we just
	 * set above, to see if it is still set.  This bit is automatically
	 * reset (zeroed) when the conversion is ready so if we do this in
	 * a loop the loop will just go until the conversion is ready. */
	while ( (ADCSRA & _BV(ADSC)) );

	/* Finally, we return the converted value to the calling function. */
	return ADC;
}

int main(void)
{
	//	foo_.init();
	//	initSPI(&receive_buffer[0],&send_buffer[0],&evaluate);  //Initialize slave SPI
	//	manager.init(&send_buffer[0],&receive_buffer[0],&evaluate);
#if 1

//	ADCSRA |= _BV(ADEN);
//	bitSet(ADMUX,REFS0);

	bitSet(DDRB,1);
	bitSet(DDRD,7);
	bitClear(PORTB,1);
	bitClear(PORTD,7);

	sei();

	control.init();
#endif
	while(true)
	{
		control.process();
	}
}
