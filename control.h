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
	void initADC();
	byte getPedalSpeed();
	void setMaxBoostPWM(byte pwm) {max_boost_pwm_ = pwm;}

private:
	int count_ = 0;
	byte pwm_ = 0;
	byte current_pwm_ = 0;
	byte max_boost_pwm_;
	double scale_pwm_;
	byte max_pwm_;

	int map(int x, int in_min, int in_max, int out_min, int out_max);
	struct InputPin : PortPin{
		bool current_state_ = true;
	};

	InputPin charge1_;
	InputPin charge2_;
	PortPin discharge1_;
	PortPin discharge2_;
};
