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

	discharge1_.setPins(&PORTC,&DDRC,&PINC,1);
	discharge2_.setPins(&PORTC,&DDRC,&PINC,2);
	charge1_.setPins(&PORTC,&DDRC,&PINC,3);
	charge2_.setPins(&PORTC,&DDRC,&PINC,4);
	discharge1_.set();
	discharge2_.set();
	charge1_.set();
	charge2_.set();
	//		engine_.setPWM(55);

	//		engine_.setMode(Engine::ON);
	//		engine_.init(&PORTD,&DDRD,7,&PORTB,&DDRB,1);
//	engine_.setPWM(0xF0);
//	engine_.setMode(Engine::CAPACITOR);
	engine_.setMode(Engine::ON);
	engine_.setCP1ChargePWM(255);
	engine_.setCP2ChargePWM(255);
	//	engine_.setCP2Charge(true);
//	engine_.setCP1Charge(true);
	//	engine_.setCP1(true);
//	engine_.setCP2(true);
	//	bitSet(DDRD,1);
	//	bitClear(PORTD,1);
}

void Control::process()
{
	bool discharge1 =  discharge1_.read();
	bool discharge2 =  discharge2_.read();
	bool disabled;

	if(discharge1 != discharge1_.current_state_){ // change
		discharge1_.current_state_ = discharge1;
		engine_.setCP1(!discharge1);
		if(discharge1){
			disabled = true;
		}
		else{
			engine_.setMode(Engine::CAPACITOR);
		}
	}

	if(discharge2 != discharge2_.current_state_){ // change
		discharge2_.current_state_ = discharge2;
		engine_.setCP2(!discharge2);
		if(discharge2){
			disabled = true;
		}
		else{
			engine_.setMode(Engine::CAPACITOR);
		}
	}


	if(disabled){
		if(discharge1 && discharge2){
			engine_.setMode(Engine::ON);
		}
	}

	bool charge1 = charge1_.read();
	bool charge2 = charge2_.read();

	if(charge1 != charge1_.current_state_){ // change
		charge1_.current_state_ = charge1;
		engine_.setCP1Charge(!charge1);
	}

	if(charge2 != charge2_.current_state_){ // change
		charge2_.current_state_ = charge2;
		engine_.setCP2Charge(!charge2);
	}

	accelerate();
	count_++;
	if(count_ == 100){
		pwm_ = getPedalSpeed();
		count_ = 0;
		if(pwm_ != current_pwm_){
			acceleration_counter_ = 1;
		}
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
	if(acceleration_counter_ && --acceleration_counter_ == 0){
		if(current_pwm_ == pwm_)
		{
			return;
		}
		if(current_pwm_ > pwm_)
		{
			current_pwm_ = pwm_;
		}
		else{
			current_pwm_++;
			if(current_pwm_ == pwm_){
				// acceleration counter is already 0

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
