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
	void setCP1(bool state);
	void setCP2(bool state);

private:
	void switchRelayOff();
	void processSwitchOff();
	void switchRelayOn();
	void processSwitchOn();
	void updatePWM();
public:

	enum Process : byte{
		START,
		enableRelay,
		disableRelay,
		enableMosfet,
		disableMosfet,
		enableCapacitor1,
		enableCapacitor2,
		disableCapacitors,
		END,
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
	};
	bool in_process_;
	bool new_new_used_;

	Settings current_settings_;
	Settings new_settings_;
	Settings new_new_settings_; // dont use in_process_

	Process *process_ptr_;
	Process processes_[8];
	int counter_;


};
