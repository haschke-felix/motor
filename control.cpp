#include "control.h"
#include "usb.h"
#include "adc.h"

Control::Control() : scale_pwm_(0.375), max_pwm_(255)
{
}

void Control::init()
{
	Engine::init(PortPin(PortPin::D, 7), PortPin(PortPin::B, 1), PortPin(PortPin::D, 2), PortPin(PortPin::D, 5));
	ADC_Init();
	Engine::setMode(Engine::ON);
	setCharge(true);
	setChargePWM(40);
}

#define MAX_PWM_INCREASE 5
void Control::process()
{
//	setMode(ON);
//	setPWM(getPedalSpeed());
//	if (getPedalSpeed() > 500)
//	{
//		bitSet(DDRB, 0);
//	}

//	else
//	{
		bitClear(DDRB, 0);
//	}
#if 1
   static int counter = 0;
	if (counter++ == 0xFF)
	{
		counter = 0;
#if 1
		if (!bitRead(PIND, 4))
		{
			Engine::setMode(Engine::CAPACITOR);
		}

		else
		{
			Engine::setMode(Engine::ON);
			bitSet(DDRB, 0);
		}
#endif
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
	}
	Engine::process();
#if 1
	if (++count_ % 10 == 0)
	{
		pwm_ = getPedalSpeed();
		// limit pwm increase per cycle
		if (pwm_ > last_pwm_ + MAX_PWM_INCREASE)
			pwm_ = last_pwm_ + MAX_PWM_INCREASE;
		// apply changes
		if (pwm_ != last_pwm_)
		{
			last_pwm_ = pwm_;
			Engine::setPWM(pwm_);
		}
	}
	if (count_ == 1000)
	{
		Engine::setPWM(pwm_);
		count_ = 0;
	}
#endif
#endif
}


byte Control::getPedalSpeed()
{
	unsigned int value = ADC_Read(0);
	if (value < 400)
	{
		return 255;
	}
	else if (value > 900)
	{
		return 0;
	}
	else
	{
		return 255-((value - 400) * 0.51);
	}
}

int Control::map(int x, int in_min, int in_max, int out_min, int out_max)
{
	int out =  (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	if(out < out_min)
		return out_min;
	else if(out > out_max)
		return out_max;
	return out;
}
