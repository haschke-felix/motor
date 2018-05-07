#pragma once
#include "avr.h"
#include "engine.h"

class Capacitor
{
public:
	 Capacitor();
	 void init(volatile byte *port_discharge, volatile byte *ddr_dicharge, byte pin_discharge,
	           volatile byte *port_charge, volatile byte *ddr_charge, byte pin_charge,
	           volatile byte *ddr_charge_mosfet, byte pin_charge_mosfet,
	           volatile byte *tccr_charge_mosfet, volatile byte *ocr_h_charge_mosfet, volatile byte *ocr_l_charge_mosfet, Engine *engine);
	 void initPWM();
	 void processPWM(byte pwm);
	 void process();
	 void processCharge(byte mode  = 0);
	 void processDischarge(byte mode = 0);
	 void setCharge(bool flag);
	 bool getCharge();
	 void setChargePWM(byte pwm){charge_pwm_ = pwm;}
	 byte getChargePWM(){return charge_pwm_;}
	 void setDischarge(bool flag); // priority mode
	 bool getDischarge();
	 int getLevel();
	 long long int getTime();



	 enum CapacitorMode : byte{
		 Discharge =  1 << 0,
		 Charge    =  1 << 1,
		 Update    =  1 << 2,
		 InProcess =  1 << 3,
	 };


private:
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

	 volatile byte * port_discharge_, *ddr_discharge_;
	 volatile byte * port_charge_, *ddr_charge_;
	 byte pin_discharge_;
	 byte pin_charge_;


	 volatile byte * ddr_charge_mosfet_;
	 byte pin_charge_mosfet_;
	 volatile byte * tccr_charge_mosfet_;
	 volatile byte * ocr_h_charge_mosfet_, *ocr_l_charge_mosfet_;

	 byte previous_mode_;
	 byte mode_;
	 byte next_mode_;
	 long long int time_; // time of charge or discharge
	 int level_;
	 byte charge_pwm_;
	 Engine * engine_;

	 int counter_;
	 Process * current_process_;
	 Process process_array_[10];
};
