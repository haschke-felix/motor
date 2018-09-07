#include "control.h"
Control::Control()
{

}

void Control::init(){
	engine_.init(PortPin::staticPortPin(PortPin::D,7),PortPin::staticPortPin(PortPin::B,1),
	             PortPin::staticPortPin(PortPin::D,0),PortPin::staticPortPin(PortPin::D,1),
	             PortPin::staticPortPin(PortPin::D,2),PortPin::staticPortPin(PortPin::D,4),
	             PortPin::staticPortPin(PortPin::D,5),PortPin::staticPortPin(PortPin::D,6));
	initADC();
	discharge1_.setPins(PortPin::C,1);
	discharge2_.setPins(PortPin::C,2);
	charge1_.setPins(PortPin::C,3);
	charge2_.setPins(PortPin::C,4);
	discharge1_.set();
	discharge2_.set();
//	discharge1_.input();
//	discharge2_.input();
	charge1_.set();
	charge2_.set();
	engine_.setPWM(0);
	engine_.setMode(Engine::ON);
	engine_.setCP1ChargePWM(15);
	engine_.setCP2ChargePWM(15);
	engine_.setCP1Charge(true);
	engine_.setCP2Charge(true);

	engine_.setCP1(true);
	engine_.setCP2(true);
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
	}
	bitWrite(DDRB,0,bitRead(PINC,2));
	engine_.process();
	if(++count_ == 100){
		pwm_ = getPedalSpeed();
		count_ = 0;
		if(pwm_ != current_pwm_){
			engine_.setPWM(pwm_);
		}
	}
}

void Control::initADC()
{
	ADCSRA |= _BV(ADEN);
	bitSet(ADMUX,REFS0);
	for(int i = 0; i < 10; i++){
		getPedalSpeed(); // initial read of adc
	}
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
	ADMUX	&=	0xf0;
	ADMUX	|=	5;
	ADCSRA |= _BV(ADSC);
	while ( (ADCSRA & _BV(ADSC)) );
	int value = ADC;

	if(value < 200){
		return 1;
	}
	if(value > 900){
		return 255;
	}
	else{
		return (value - 200) * 0.364285714;
	}
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
