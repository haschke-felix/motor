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
Engine engine;
//Control control;

int main(void)
{
	//	foo_.init();
	//	initSPI(&receive_buffer[0],&send_buffer[0],&evaluate);  //Initialize slave SPI
	//	manager.init(&send_buffer[0],&receive_buffer[0],&evaluate);
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
	engine.init(&PORTD,&DDRD,7,&PORTB,&DDRB,1);
	engine.setPWM(0xF0);
	engine.setMode(Engine::ON);
	while(true)
	{
		engine.setPWM(50);
		engine.process();
	}
}
