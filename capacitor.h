#pragma once
#include "avr.h"
#include "engine.h"
#include "portpin.h"

class Capacitor
{
public:
	Capacitor();
	void init(PortPin charge, PortPin charge_mosfet, volatile byte *pwm_pin, Engine *engine);
	void processPWM(byte pwm);
	void process();
	void processCharge(byte mode  = 0);
	void processDischarge(byte mode = 0);
	void setCharge(bool flag);
	bool getCharge();
	void setChargePWM(byte pwm){settings_.charge_pwm_ = pwm;}
	byte getChargePWM(){return settings_.charge_pwm_;}
	void setDischarge();
	bool getDischarge();
	int getLevel();
	long long int getTime();

	enum CapacitorMode : byte{
		Disabled,
		Discharge,
		Charge,
	};


private:
	void initPWM();

	enum Process{
		Nothing,
		ShutdownEngineBattery, // first Phase, decouple Engine from Battery
		ClearDischarge,  // enable
		StartEngine,

		ShutdownEngine,
		SetDischarge,   // disable
		StartEngineBattery,

		SetChargeMosfet, // disable
		SetCharge,       // disable

		ClearCharge,	   //enable
		ClearChargeMosfet, // enable
	};

	struct Settings{
		CapacitorMode mode_ = Disabled;
		bool charge_pwm_ = 50;
	};

	Settings settings_;

	PortPin charge_mosfet_;
	PortPin charge_;
	volatile byte *pwm_pin_;

	long long int time_; // time of charge or discharge
	int level_;

	Engine * engine_;

	int counter_;
};
