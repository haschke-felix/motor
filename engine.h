#pragma once
#include "avr.h"
#include "Vportpin.h"

class Engine
{
public:
	Engine();
	enum EngineMode : byte{
		ON,
		OFF,
		CAPACITOR,
	};

    void init(PortPin motor_vcc, PortPin motor_pwm, PortPin cp1, PortPin cp2, PortPin cp1_charge, PortPin cp2_charge, PortPin cp1_charge_mosfet, PortPin cp2_charge_mosfet);
	void process();
	void initPWM();
	void setPWM(byte pwm);
	byte getPWM();
	byte getCurrentPWM(){return current_settings_.pwm_;}
	void setMode(EngineMode mode);
	EngineMode getCurrentMode(){return current_settings_.mode_;}
	EngineMode getMode();
	bool getInProcess() {return in_process_;}
	void setCP1(bool state);
	void setCP2(bool state);
	void setCP1Charge(bool state);
	void setCP2Charge(bool state);
	void setCP1ChargePWM(byte pwm);
	void setCP2ChargePWM(byte pwm);


private:
	void startProcess();
	void processing();

	void processPWM(byte pwm);
	void processPwmCp2(byte pwm);
	void processPwmCp1(byte pwm);

	enum Process : byte{
		START,
		enableRelay,
		disableRelay,
		enableMosfet,
		disableMosfet,
		enableCapacitor1,
		enableCapacitor2,
		disableCapacitor1,
		disableCapacitor2,
		enableChargeCapacitor1,
		enableChargeCapacitor2,
		disableChargeCapacitor1,
		disableChargeCapacitor2,
		enableChargeMosfet1,
		enableChargeMosfet2,
		disableChargeMosfet1,
		disableChargeMosfet2,
		END,
	};

	PortPin motor_pwm_;
	PortPin motor_vcc_;
	PortPin cp1_;
	PortPin cp2_;
	PortPin cp1_charge_;
	PortPin cp2_charge_;
	PortPin cp1_charge_mosfet_;
	PortPin cp2_charge_mosfet_;

	struct Settings
	{
		byte pwm_ = 0;
		EngineMode mode_ = OFF;
		bool cp1_ = false;
		bool cp2_ = false;
		bool cp1_charge_ = false;
		bool cp2_charge_ = false;
		byte cp1_charge_pwm_ = 0;
		byte cp2_charge_pwm_ = 0;
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
