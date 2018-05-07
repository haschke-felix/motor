#pragma once
#include "avr.h"

class Engine
{
public:
	Engine();
	enum EngineMode : byte{
		ON,
		OFF,
		CAPACITOR,
	};

	void init(volatile byte *port_motor_vcc, volatile byte * ddr_motor_vcc, byte pin_motor_vcc,
	          volatile byte * port_motor_pwm, volatile byte * ddr_motor_pwm, byte pin_motor_pwm);
	void process();
	void initPWM();
	void processPWM(byte pwm);
	void setPWM(byte pwm);
	byte getPWM(){return current_settings_.pwm_;}
	void setMode(EngineMode mode);
	EngineMode getMode(){return current_settings_.mode_;}

	void switchRelayOff();
	void processSwitchOff();
	void switchRelayOn();
	void processSwitchOn();
	void updatePWM();
	void setCP1(bool state);
	void setCP2(bool state);


	enum Process : byte{
		setRelay,
		setMosfet,
		clearMosfet,
		clearRelay,
		Nothing,
	};

private:

	struct PortPin
	{
		volatile byte *port_, *ddr_;
		byte pin_;
	};

	PortPin motor_pwm_;
	PortPin motor_vcc_;

	struct Settings
	{
		byte pwm_;
		EngineMode mode_ = OFF;
		bool cp1_;
		bool cp2_;
		bool in_process_;
	};

	Settings current_settings_;
	Settings new_settings_;
	Settings new_new_settings_; // dont use in_process_

	Process process_;
	int counter_;


};
