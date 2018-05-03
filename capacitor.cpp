#include "capacitor.h"
Capacitor::Capacitor()
{

}

void Capacitor::init(volatile byte *port_discharge, volatile byte *ddr_dicharge, byte pin_discharge,
                     volatile byte *port_charge, volatile byte *ddr_charge, byte pin_charge,
                     volatile byte *ddr_charge_mosfet, byte pin_charge_mosfet, volatile byte *tccr_charge_mosfet,
                     volatile byte *ocr_h_charge_mosfet, volatile byte *ocr_l_charge_mosfet, Engine * engine)
{
	engine_ = engine;

	// discharge pin declaration
	port_discharge_ = port_discharge;
	ddr_discharge_ = ddr_dicharge;
	pin_discharge_ = pin_discharge;

	// charge pin declaration
	port_charge_ = port_charge;
	ddr_charge_ = ddr_charge;
	pin_charge_ = pin_charge;

	// charge mosfet pwm pin declaration
	ddr_charge_mosfet_ = ddr_charge_mosfet;
	pin_charge_mosfet_ = pin_charge_mosfet;
	tccr_charge_mosfet_ = tccr_charge_mosfet;
	ocr_h_charge_mosfet_ = ocr_h_charge_mosfet;
	ocr_l_charge_mosfet_ = ocr_l_charge_mosfet;

	// init discharge pin: disable(low active) and set as Ouput)
	bitSet(*port_discharge_,pin_discharge_);
	bitSet(*ddr_discharge_,pin_discharge_);
	// init charge pin: disable(low active) and set as Ouput)
	bitSet(*port_charge_,pin_charge_);
	bitSet(*ddr_charge_,pin_charge_);

	initPWM();
}

void Capacitor::initPWM()
{
	//	turn off interrupts (is needed to write OCR1A usw)
//	cli();
	bitSet(*ddr_charge_mosfet_,pin_charge_mosfet_);
	bitSet(*tccr_charge_mosfet_,0);
	bitSet(*tccr_charge_mosfet_,(pin_charge_mosfet_ == 5 ? 7 : 5));
	*tccr_charge_mosfet_ |=(1<<CS11); // clock / 8
	sei();
}

void Capacitor::process()
{
	if(mode_ & InProcess){
		processDischarge();
		processCharge();
	}
	else if(mode_ & Update){
		processDischarge();
		processCharge();
		mode_ &= ~ Update;
	}
}

void Capacitor::processCharge()
{
	if(mode_ == InProcess){
		return;
	}
	bool charge = mode_ & Charge && !(mode_ & Discharge);

	// toggle relay
	bitWrite(*port_charge_,pin_charge_,charge);


	// writeChargePWM
	*ocr_h_charge_mosfet_ |=0x00;
	*ocr_l_charge_mosfet_ |= charge_pwm_;
	sei();	//setting PWM-Ports to output is needed
}

void Capacitor::processDischarge()
{
	if(mode_ & Discharge){
		mode_ |= InProcess;
	}
}

void Capacitor::setCharge(bool flag)
{
	if(getCharge() != flag){
		if(flag){
			mode_ |= Charge;
		}
		else{
			mode_ &= ~ Charge;
		}
		mode_ &= Update;
		process();
	}
}

bool Capacitor::getCharge()
{

}

void Capacitor::setDischarge(bool flag)
{
	if(getDischarge() != flag){
		if(flag){
			mode_ |= Discharge;
		}
		else{
			mode_ &= ~ Discharge;
		}
		mode_ &= Update;
		process();
	}
}

bool Capacitor::getDischarge()
{
	return mode_ & Discharge;
}

int Capacitor::getLevel()
{
	return level_;
}

long long Capacitor::getTime()
{
	return time_;
}
