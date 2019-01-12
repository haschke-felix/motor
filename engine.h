#pragma once
#include "Vportpin.h"
#include "avr.h"

class Engine
{
public:
	Engine();
	enum EngineMode : byte
	{
		ON,
		OFF,
		CAPACITOR,
	};

	void init(PortPin motor_vcc, PortPin motor_pwm, PortPin charge_relay, PortPin charge_mosfet);
	void process();
	void initPWM();
	void setPWM(byte pwm);
	byte getPWM();
	byte getCurrentPWM()
	{
		return current_settings_.pwm_;
	}
	void setMode(EngineMode mode);
	EngineMode getCurrentMode()
	{
		return current_settings_.mode_;
	}
	EngineMode getMode();
	bool getInProcess()
	{
		return in_process_;
	}
	void setCharge(bool state);
	void setChargePWM(byte pwm);

private:
	void initProcess();
	void processing();

	void processPWM(byte pwm);
	//	void processPwmCp2(byte pwm);
	void processChargePwm(byte pwm);

	enum Process : byte
	{
		START,
		enableRelay,
		disableRelay,
		enableMosfet,
		disableMosfet,
		enableCapacitor,
		disableCapacitor,
		enableChargeMosfet,
		disableChargeMosfet,
		END,
	};

	PortPin motor_pwm_;
	PortPin motor_vcc_;
	PortPin charge_relay_;
	PortPin charge_mosfet_;

	struct Settings
	{
		byte pwm_ = 0;
		EngineMode mode_ = OFF;
		bool charge_ = false;
		byte charge_pwm_ = 0;
	};
	bool in_process_ = false;
	bool new_new_used_;

	Settings current_settings_;
	Settings new_settings_;
	Settings new_new_settings_; // dont use in_process_

	Process *process_ptr_;
	Process processes_[12];
	int counter_ = 1;
};
