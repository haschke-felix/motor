#include "control.h"
Control::Control()
{

}

void Control::init(){
	engine_.init(PortPins::set(&PORTD,&DDRD,7),PortPins::set(&PORTB,&DDRB,1),
	             PortPins::set(&PORTD,&DDRD,0),PortPins::set(&PORTD,&DDRD,1),
	             PortPins::set(&PORTD,&DDRD,2),PortPins::set(&PORTD,&DDRD,4),
	             PortPins::set(&PORTD,&DDRD,5),PortPins::set(&PORTD,&DDRD,6));

	initADC();

	discharge1_.port_pin_ = PortPins::set(&PORTC,&DDRC,&PINC,1);
	discharge2_.port_pin_ = PortPins::set(&PORTC,&DDRC,&PINC,2);
	charge1_.port_pin_ = PortPins::set(&PORTC,&DDRC,&PINC,3);
	charge2_.port_pin_ = PortPins::set(&PORTC,&DDRC,&PINC,4);
	bitSet(*discharge1_.port_pin_.port,discharge1_.port_pin_.pin);
	bitSet(*discharge2_.port_pin_.port,discharge2_.port_pin_.pin);
	bitSet(*charge1_.port_pin_.port,charge1_.port_pin_.pin);
	bitSet(*charge2_.port_pin_.port,charge2_.port_pin_.pin);
	//		engine_.setPWM(55);

	//		engine_.setMode(Engine::ON);
	//		engine_.init(&PORTD,&DDRD,7,&PORTB,&DDRB,1);
	engine_.setPWM(0xF0);
	engine_.setMode(Engine::CAPACITOR);
	engine_.setMode(Engine::ON);
	engine_.setCP1ChargePWM(255);
	//	engine_.setCP2ChargePWM(255);
	//	engine_.setCP2Charge(true);
	engine_.setCP1Charge(true);
	//	engine_.setCP1(true);
	engine_.setCP2(true);
	//	bitSet(DDRD,1);
	//	bitClear(PORTD,1);
}

void Control::process()
{
	bool discharge1 =  bitRead(*discharge1_.port_pin_.in_pin,discharge1_.port_pin_.pin);
	if(discharge1 != discharge1_.current_state_){ // change
		if(discharge1){

		}
	}

	accelerate();
	count_++;
	if(count_ == 100){
		pwm_ = getPedalSpeed();
		count_ = 0;
	}
	engine_.process();
}

void Control::initADC()
{
	ADCSRA |= _BV(ADEN);
	bitSet(ADMUX,REFS0);
}

void Control::accelerate()
{
	if(acceleration_counter_ && !(--acceleration_counter_)){
		if(current_pwm_ == pwm_)
		{
			acceleration_counter_ = 0x00;
			return;
		}
		if(current_pwm_ > pwm_)
		{
			acceleration_counter_ = 0x00;
			current_pwm_ = pwm_;
		}
		else{
			current_pwm_++;
			if(current_pwm_ == pwm_){
				acceleration_counter_ = 0x00;

			}
			else{
				int acceleration = 0xFF - current_pwm_ + 1;
				acceleration_counter_ = acceleration;
			}
		}
		engine_.setPWM(current_pwm_);
	}
}

byte Control::getPedalSpeed()
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
