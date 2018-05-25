#include "capacitor.h"
Capacitor::Capacitor()
{

}

void Capacitor::init(PortPin charge, PortPin charge_mosfet, volatile byte *pwm_pin, Engine * engine)
{
	engine_ = engine;

	charge_ = charge;
	charge_mosfet_ = charge_mosfet;
	pwm_pin_ = pwm_pin;

	// init charge pin: disable(low active) and set as Ouput)
	bitSet(*charge_.port,charge_.pin);
	bitSet(*charge_.ddr,charge_.pin);

	bitSet(*charge_mosfet_.ddr,charge_mosfet_.pin);

	initPWM();
	processPWM(50);
}

void Capacitor::initPWM()
{
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
	// set none-inverting mode

	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	// set fast PWM Mode

	TCCR0B |= (1 << CS01);
	*pwm_pin_ = 0xFF; // out at OC1A
	sei();
}

void Capacitor::processPWM(byte pwm)
{
	// writeChargePWM
	*pwm_pin_ = 0xFF - pwm;
}

void Capacitor::process()
{
//	if(mode_ & InProcess && counter_ && --counter_ == 0){
//		processDischarge(Update);
//		processCharge(Update);
//	}
    //	else if(mode_ & Update && next_process_ == Nothing){
    //		if(mode_ & Discharge){
    //			next_process_ =
    //		}
    //		else if(mode_ & Charge){ // check charge

    //		}
    //		mode_ &= ~ Update;
    //	++}
}

void Capacitor::processCharge(byte mode)
{
//	if(mode_ & InProcess){
//		if(mode_ & Charge  && ! mode_ & Discharge){
//			if(*current_process_ == SetChargeMosfet){
//				processPWM(0);
//				counter_ = 100;
//				*current_process_ = SetCharge;
//			}

//			else if(*current_process_ == SetCharge){
//				bitSet(*charge_.port,charge_.pin);
//				*current_process_ = Nothing;
//			}
//		}

//		else if(! mode_ & Charge || mode_ & Discharge){
//			if(*current_process_ == ClearCharge){
//				bitClear(*charge_.port,charge_.pin);
//				counter_ = 100;
//				*current_process_ = ClearChargeMosfet;
//			}
//			else if(*current_process_ == ClearChargeMosfet){
//				processPWM(charge_pwm_);
//				*current_process_ = Nothing;
//			}
//		}
//		return;
//	}
}


void Capacitor::processDischarge(byte mode)
{
//	if(mode == Update && current_process_ == &process_array_[0]){
//		if(previous_mode_ & Discharge ^ mode_ & Discharge){
//			if(mode_ & Discharge){
//				mode_ = InProcess;
//				process_array_[0] = ShutdownEngineBattery;
//				process_array_[1] = ClearDischarge;
//			}

//		}
//	}
//	else if(mode == 0){
//		if(mode_ & Discharge){
//			mode_ |= InProcess;
//		}
//	}

//	else if(mode_ & InProcess){

//	}
}

void Capacitor::setCharge(bool flag)
{
//	if(getCharge() != flag){
//		byte *mode;
//		if(*current_process_ != Nothing){
//			mode = &mode_;
//		}
//		else{
//			mode = &next_mode_;
//		}
//		if(flag){
//			*mode |= Charge;
//		}
//		else{
//			*mode &= ~ Charge;
//		}
//		*mode &= Update;
//		process();
//	}
}

bool Capacitor::getCharge()
{

}

void Capacitor::setDischarge()
{
//	if(getDischarge() != flag){
//		if(flag){
//			mode_ |= Discharge;
//		}
//		else{
//			mode_ &= ~ Discharge;
//		}
//		mode_ &= Update;
//		process();
//	}
}

bool Capacitor::getDischarge()
{
//	return mode_ & Discharge;
}

int Capacitor::getLevel()
{
	return level_;
}

long long Capacitor::getTime()
{
	return time_;
}
