#pragma once
#include "avr.h"
#include "engine.h"
#include "Vportpin.h"

class Control : public Engine
{
public:
	Control();
	void init();
	void process();
	void writeEnginePWM();
	void initADC();
	void accelerate();
	byte getPedalSpeed();

private:
	int count_;
	byte acceleration_counter_;
	byte pwm_;
	byte current_pwm_;
	int map(int x, int in_min, int in_max, int out_min, int out_max);
	struct InputPin : PortPin{
		bool current_state_ = true;
	};

	InputPin charge1_;
	InputPin charge2_;
	PortPin discharge1_;
	PortPin discharge2_;
};
