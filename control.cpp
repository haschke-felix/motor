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
//	discharge1_.input();
//	discharge2_.input();
	charge1_.set();
	charge2_.set();
	//	engine_.setPWM(55);

	//		engine_.setMode(Engine::ON);
	//		engine_.init(&PORTD,&DDRD,7,&PORTB,&DDRB,1);
	//	  engine_.setPWM(50);
	engine_.setMode(Engine::ON);
	//	engine_.setMode(Engine::CAPACITOR);
	engine_.setCP1ChargePWM(30);
	engine_.setCP2ChargePWM(100);
	//		engine_.setCP2Charge(true);
	engine_.setCP1Charge(true);
	engine_.setCP2Charge(true);

	engine_.setCP1(true);
	engine_.setCP2(true);
	//		bitSet(DDRD,0);
	//		bitClear(PORTD,0);
}

void Control::process()
{
	static int counter = 0;

	if(counter++ == 0xFFF){
		counter = 0;

		static bool boost_state1 =  false;
		static bool boost_state2 =  false;

		bool boost1 = discharge1_.read();
		bool boost2 = discharge2_.read();
		bool motor = false; // false is only motor, true is capacitor
		static bool motor_state = false;

		//	if(boost_state1 != boost1 || boost_state2 != boost2){
		//		if((!boost1) || (!boost2)){
		//			motor = true;
		//		}
		//		if(motor_state != motor){
		//			if(motor_state){
		//				engine_.setMode(Engine::CAPACITOR);
		//			}
		//			else{
		//				engine_.setMode(Engine::ON);
		//			}
		//		}
		//		else{
		//			engine_.setCP1(!boost1);
		//			engine_.setCP2(!boost2);
		//		}
		//	}
		if(boost1 && boost2){
			engine_.setMode(Engine::ON);
//			engine_.setCP1Charge(false);
		}
		else{
//			engine_.setCP1Charge(true);
			engine_.setMode(Engine::CAPACITOR);
			engine_.setCP1(!boost1);
			engine_.setCP2(!boost2);
		}
//		bitToggle(DDRB,0);

	}
	bitWrite(DDRB,0,bitRead(PINC,2));
#if 0

	if(discharge1 != discharge1_.current_state_){ // change
		discharge1_.current_state_ = discharge1;
		engine_.setCP1(!discharge1);
		engine_.setPWM(current_pwm_);
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
		engine_.setPWM(current_pwm_);

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
#endif
	//	accelerate();
	engine_.process();
	if(++count_ == 100){
		pwm_ = getPedalSpeed();
		count_ = 0;
		if(pwm_ != current_pwm_){
			//			acceleration_counter_ = 1;
			engine_.setPWM(pwm_);
		}
	}
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
			// acceleration counter is already 0


			if(current_pwm_ == pwm_){}
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
	//	return map(ADC,0,1023,0,255);
	return ADC/4;
}

int Control::map(int x, int in_min, int in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Control::setMode(Engine::EngineMode mode)
{
	engine_.setMode(mode);
}

void Control::setCP1(bool state)
{
	engine_.setCP1(state);
}

void Control::setCP2(bool state)
{
	engine_.setCP2(state);
}

void Control::setCP1Charge(bool state)
{
	engine_.setCP1Charge(state);
}

void Control::setCP2Charge(bool state)
{
	engine_.setCP2Charge(state);
}

void Control::setCP1ChargePWM(byte pwm)
{
	engine_.setCP1ChargePWM(pwm);
}

void Control::setCP2ChargePWM(byte pwm)
{
	engine_.setCP2ChargePWM(pwm);
}
