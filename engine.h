#pragma once
#include "avr.h"

class Engine
{
public:
	Engine();
	enum EngineMode : byte{
		ON,
		OFF,
		Capacitor,
		InProcess
	};

	void init(volatile byte * port_motor_pwm, volatile byte * ddr_motor_pwm, byte pin_motor_pwm,
	          volatile byte *port_motor_vcc, volatile byte * ddr_motor_vcc, byte pin_motor_vcc);
	void process();
	void processPWM(byte pwm);
	void setPWM(byte pwm);
	byte getPWM(){return pwm_;}
	void setMode(EngineMode mode);
	EngineMode getMode(){return mode_;}

	void switchOff();
	void processSwitchOff();
	void switchOn();
	void processSwitchOn();
	void updatePWM();


	enum Process : byte{
		setRelay,
		setMosfet,
		clearMosfet,
		clearRelay,
		Nothing,
	};

private:

	volatile byte *port_motor_pwm_, *ddr_motor_pwm_;
	byte pin_motor_pwm_;
	volatile byte *port_motor_vcc_, *ddr_motor_vcc_;
	byte pin_motor_vcc_;

	EngineMode mode_;
	Process next_process_;
	int counter_;

	byte pwm_;

};
