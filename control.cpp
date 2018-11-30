#include "control.h"
Control::Control(): scale_pwm_(0.375), max_pwm_(255)
{
}

void Control::init(){
    Engine::init(PortPin(PortPin::D,7),PortPin(PortPin::B,1),
                 PortPin(PortPin::D,0),PortPin(PortPin::D,1),
                 PortPin(PortPin::D,2),PortPin(PortPin::D,4),
                 PortPin(PortPin::D,5),PortPin(PortPin::D,6));
	initADC();
//	discharge1_.setPins(PortPin::C,1);
//	discharge2_.setPins(PortPin::C,2);
//	charge1_.setPins(PortPin::C,3);
//	charge2_.setPins(PortPin::C,4);
//	discharge1_.set();
//	discharge2_.set();
//	charge1_.set();
//	charge2_.set();
	Engine::setPWM(0);
	Engine::setMode(Engine::ON);
//	Engine::setCP1ChargePWM(15);
//	Engine::setCP2ChargePWM(15);
//	Engine::setCP1Charge(true);
//	Engine::setCP2Charge(true);

//	Engine::setCP1(true);
//	Engine::setCP2(true);
}

#define MAX_PWM_INCREASE 5
void Control::process()
{
	static int counter = 0;

//    if(counter++ == 0xFF){
//		counter = 0;

//		static bool boost_state1 =  false;
//		static bool boost_state2 =  false;

//		bool boost1 = discharge1_.read();
//		bool boost2 = discharge2_.read();
//		if(boost1 && boost2){
//			Engine::setMode(Engine::ON);
//			max_pwm_ = 255;
//			scale_pwm_ = 0.375;
//		}
//		else{
//			if(max_boost_pwm_ < 233){
//				max_pwm_ = 233;
//				scale_pwm_ = 0.342647059;
//			}
//			else{
//				max_pwm_ = max_boost_pwm_;
//				scale_pwm_ = max_boost_pwm_ / 680.0;
//			}
//			Engine::setMode(Engine::CAPACITOR);
//			Engine::setCP1(!boost1);
//			Engine::setCP2(!boost2);
//		}
//        bitWrite(DDRB,0,bitRead(PINC,2));
//	}
	Engine::process();
	if(++count_	% 10 == 0){
		pwm_ = getPedalSpeed();
		// limit pwm increase per cycle
		if(pwm_ > last_pwm_ + MAX_PWM_INCREASE)
			pwm_ = last_pwm_ + MAX_PWM_INCREASE;
		// apply changes
		if(pwm_ != last_pwm_){
			last_pwm_ = pwm_;
			Engine::setPWM(pwm_);
		}
	}
	if(count_ == 1000){
		Engine::setPWM(pwm_);
		count_ = 0;
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

byte Control::getPedalSpeed()
{
	ADMUX	&=	0xf0;
	ADMUX	|=	5;
	ADCSRA |= _BV(ADSC);
	while ( (ADCSRA & _BV(ADSC)) );
    unsigned int value = ADC;
    if(value < 60){
		return 0;
	}
    else if(value > 700){
        return 255;
	}
	else{
        return (value - 60) * 0.3984375;
	}
}

int Control::map(int x, int in_min, int in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
