#include "control.h"
Control::Control()
{
}

void Control::init(){
	Engine::init(PortPin::create(PortPin::D,7),PortPin::create(PortPin::B,1),
	             PortPin::create(PortPin::D,0),PortPin::create(PortPin::D,1),
	             PortPin::create(PortPin::D,2),PortPin::create(PortPin::D,4),
	             PortPin::create(PortPin::D,5),PortPin::create(PortPin::D,6));
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
	Engine::setPWM(0);
	Engine::setMode(Engine::ON);
	Engine::setCP1ChargePWM(15);
	Engine::setCP2ChargePWM(15);
	Engine::setCP1Charge(true);
	Engine::setCP2Charge(true);

	Engine::setCP1(true);
	Engine::setCP2(true);
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
			Engine::setMode(Engine::ON);
//			Engine::setCP1Charge(false);
		}
		else{
//			Engine::setCP1Charge(true);
			Engine::setMode(Engine::CAPACITOR);
			Engine::setCP1(!boost1);
			Engine::setCP2(!boost2);
		}
	}
	bitWrite(DDRB,0,bitRead(PINC,2));
	Engine::process();
	if(++count_ == 100){
		pwm_ = getPedalSpeed();
		count_ = 0;
		if(pwm_ != current_pwm_){
			Engine::setPWM(pwm_);
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
		Engine::setPWM(current_pwm_);
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
