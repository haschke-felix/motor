#pragma once
#include "Vportpin.h"
#include "avr.h"
#include "list.h"

class Engine
{
public:
	Engine();
	~Engine();
	enum EngineMode : byte
	{
		ON,
		OFF,
		CAPACITOR,
	};

	void init(PortPin motor_vcc, PortPin motor_pwm, PortPin capacitor_relay, PortPin charge_relay, PortPin charge_mosfet);
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

	void processPWM(const byte pwm);
	//	void processPwmCp2(byte pwm);
	void processChargePwm(const byte pwm);

	enum Process : byte
	{
		UNUSED,
		START,
		enableRelay,
		disableRelay,
		enableMosfet,
		disableMosfet,
		enableCapacitor,
		disableCapacitor,
		enableChargeCapacitor,
		disableChargeCapacitor,
		enableChargeMosfet,
		disableChargeMosfet,
		END,
	};

	PortPin motor_pwm_;
	PortPin motor_vcc_;
	PortPin capacitor_relay_;
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

	List<Process> process_list_;

	int counter_ = 1;
};
