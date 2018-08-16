#pragma once
#include "avr.h"
#include "engine.h"
#include "portpin.h"

class Control
{
public:



	 Control();
	 void init();
	 void process();
	 void writeEnginePWM();
	 void initADC();
	 void accelerate();
	 byte getPedalSpeed();


	 byte getCurrentPWM(){}
	 void setMode(Engine::EngineMode mode);
	 void setCP1(bool state);
	 void setCP2(bool state);
	 void setCP1Charge(bool state);
	 void setCP2Charge(bool state);
	 void setCP1ChargePWM(byte pwm);
	 void setCP2ChargePWM(byte pwm);

private:
	 Engine  engine_;
	 int count_;
	 byte acceleration_counter_;
	 byte pwm_;
	 byte current_pwm_;
	 struct InputPin : PortPin{
		 bool current_state_ = true;
	 };

	 InputPin charge1_;
	 InputPin charge2_;
	 InputPin discharge1_;
	 InputPin discharge2_;
};
