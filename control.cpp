#include "control.h"

Control::Control()
{

}

void Control::init(){
	engine_.init(PortPins::set(&PORTD,&DDRD,7),PortPins::set(&PORTB,&DDRB,1),PortPins::set(&PORTD,&DDRD,0),PortPins::set(&PORTD,&DDRD,1));
	   cp1_.init(PortPins::set(&PORTD,&DDRD,2),PortPins::set(&PORTD,&DDRD,5),&OCR0A,&engine_);
		cp2_.init(PortPins::set(&PORTD,&DDRD,4),PortPins::set(&PORTD,&DDRD,6),&OCR0B,&engine_);

	initADC();
	//		engine_.setPWM(55);

	//		engine_.setMode(Engine::ON);
	//		engine_.init(&PORTD,&DDRD,7,&PORTB,&DDRB,1);
	engine_.setPWM(0xF0);
//	engine_.setMode(Engine::OFF);
	engine_.setMode(Engine::ON);
}

void Control::process()
{
	engine_.process();
	cp1_.process();
	cp2_.process();

	count_++;
	if(count_ == 100){
		engine_.setPWM(getSpeedPedal());
		count_ = 0;
	}
}

void Control::initADC()
{
	ADCSRA |= _BV(ADEN);
	bitSet(ADMUX,REFS0);
}

byte Control::getSpeedPedal()
{
	/* adcx is the analog pin we want to use.  ADMUX's first few bits are
		 * the binary representations of the numbers of the pins so we can
		 * just 'OR' the pin's number with ADMUX to select that pin.
		 * We first zero the four bits by setting ADMUX equal to its higher
		 * four bits. */
	ADMUX	&=	0xf0;
	ADMUX	|=	5;  // <---------

	/* This starts the conversion. */
	ADCSRA |= _BV(ADSC);

	/* This is an idle loop that just wait around until the conversion
		 * is finished.  It constantly checks ADCSRA's ADSC bit, which we just
		 * set above, to see if it is still set.  This bit is automatically
		 * reset (zeroed) when the conversion is ready so if we do this in
		 * a loop the loop will just go until the conversion is ready. */
	while ( (ADCSRA & _BV(ADSC)) );

	/* Finally, we return the converted value to the calling function. */
	return ADC / 4;
}
