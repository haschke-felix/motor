#include "capacitor.h"

Capacitor::Capacitor()
{

}

void Capacitor::init(volatile byte *port_discharge, volatile byte *ddr_dicharge, byte pin_discharge,
                     volatile byte *port_charge, volatile byte *ddr_charge, byte pin_charge,
                     volatile byte *ddr_charge_mosfet, byte pin_charge_mosfet, volatile byte *tccr_charge_mosfet,
                     volatile byte *ocr_h_charge_mosfet, volatile byte *ocr_l_charge_mosfet)
{
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

}

void Capacitor::process()
{
	if(mode_ & Update){
		processDischarge();
		processCharge();
		mode_ &= ~ Update;
	}
}

void Capacitor::processCharge()
{
	bool charge = mode_ & Charge && !(mode_ & Discharge);

	// toggle relay
	bitWrite(*port_charge_,pin_charge_,charge);

	// writeChargePWM



	//setting PWM-Ports to output is needed
//	// 8-Bit no-inverting PWM on OC1A,OC1B,OC1C & OC3A
	TCCR1A|=0xA9;
	TCCR1B|=(1<<CS11);

//	//set Motor Speed


//	//turn off interrupts (is needed to write OCR1A usw)
	cli();
	OCR1AH|=0x00;
	OCR1AL |= charge_pwm_;
//	OCR1BH|=0x00;
//	OCR1BL|=motorSpeed2;
//	OCR1CH|=0x00;
//	OCR1CL|=motorSpeed3;
//	OCR3AH|=0x00;
//	OCR3AL|=motorSpeed4;
	//turn on interrupts
	sei();
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
