#include "engine.h"

Engine::Engine()
{

}

void Engine::init(PortPin motor_vcc, PortPin motor_pwm, PortPin cp1, PortPin cp2, PortPin cp1_charge, PortPin cp2_charge,PortPin cp1_charge_mosfet, PortPin cp2_charge_mosfet)
{
	// charge mosfet pwm pin declaration
	motor_vcc_ = motor_vcc;
	motor_pwm_ = motor_pwm;

	cp1_ = cp1;
	cp2_ = cp2;
	cp1_charge_ = cp1_charge;
	cp2_charge_ = cp2_charge;
	cp1_charge_mosfet_ = cp1_charge_mosfet;
	cp2_charge_mosfet_ = cp2_charge_mosfet;

	// init mosfet pins
	motor_pwm_.set();
	motor_pwm_.output();

	// init relay pins
	motor_vcc_.set();
	motor_vcc_.output();

	cp1_charge_mosfet_.output();
	cp2_charge_mosfet_.output();

	cp1_charge_.set();
	cp1_charge_.output();

	cp2_charge_.set();
	cp2_charge_.output();

	// init Capacitors Pins
	cp1_.set();
	cp1_.output();

	cp2_.set();
	cp2_.output();

	initPWM();
	processPWM(0);
}


void Engine::process()
{
	if(in_process_)
	{
		if(*process_ptr_ == START)
		{
			startProcess();
		}
		else{
			if(counter_ && --counter_ == 0)
			{
				processing();
			}
		}
	}
}

void Engine::startProcess()
{
	byte process_counter = 1;

	// ********************************
	if(new_settings_.mode_ == ON)
	{
		bool set = false;
		if(current_settings_.mode_ ==  CAPACITOR) // mode before was capacitor
		{
			processes_[process_counter++] = disableMosfet;
			processes_[process_counter++] = disableCapacitor1;
			processes_[process_counter++] = disableCapacitor2;
			set = true;
		}
		else if(current_settings_.mode_ == OFF){
			set = true;
		}
		if(current_settings_.cp1_charge_ != new_settings_.cp1_charge_ || set){
			if(new_settings_.cp1_charge_){
				processes_[process_counter++] = enableChargeCapacitor1;
				processes_[process_counter++] = enableChargeMosfet1;
			}
			else{
				processes_[process_counter++] = disableChargeCapacitor1;
				processes_[process_counter++] = disableChargeMosfet1;
			}
		}
		if(current_settings_.cp2_charge_ != new_settings_.cp2_charge_ || set){
			if(new_settings_.cp2_charge_){
				processes_[process_counter++] = enableChargeCapacitor2;
				processes_[process_counter++] = enableChargeMosfet2;
			}
			else{
				processes_[process_counter++] = disableChargeCapacitor2;
				processes_[process_counter++] = disableChargeMosfet2;
			}
		}
		if(set){
			processes_[process_counter++] = enableRelay;
			processes_[process_counter++] = enableMosfet;
		}
	}
	// ********************************
	else if(new_settings_.mode_ == OFF)
	{
		if(current_settings_.mode_ != OFF)
		{
			processes_[process_counter++] = disableMosfet;
			processes_[process_counter++] = disableRelay;
			if(current_settings_.mode_ == CAPACITOR)
			{
				processes_[process_counter++] = disableCapacitor1;
				processes_[process_counter++] = disableCapacitor2;
			}
		}
		// charge capacitor settings
		if(current_settings_.cp1_charge_ != new_settings_.cp1_charge_){
			if(new_settings_.cp1_charge_){
				processes_[process_counter++] = enableChargeCapacitor1;
				processes_[process_counter++] = enableChargeMosfet1;
			}
			else{
				processes_[process_counter++] = disableChargeCapacitor1;
				processes_[process_counter++] = disableChargeMosfet1;
			}
		}
		if(current_settings_.cp2_charge_ != new_settings_.cp2_charge_){
			if(new_settings_.cp2_charge_){
				processes_[process_counter++] = enableChargeCapacitor2;
				processes_[process_counter++] = enableChargeMosfet2;
			}
			else{
				processes_[process_counter++] = disableChargeCapacitor2;
				processes_[process_counter++] = disableChargeMosfet2;
			}
		}
	}

	// *******************************
	else if(new_settings_.mode_ == CAPACITOR)
	{
		if(current_settings_.mode_ == CAPACITOR)
		{
			// check currently setted caps
			bool change_cp1 = new_settings_.cp1_ ^ current_settings_.cp1_;
			bool change_cp2 = new_settings_.cp2_ ^ current_settings_.cp2_;
			if(change_cp1 || change_cp2){ // change
				processes_[process_counter++] = disableMosfet;
				if(change_cp1)
				{
					if(new_new_settings_.cp1_){
						if(new_settings_.cp1_charge_){
							processes_[process_counter++] = disableChargeMosfet1;
							processes_[process_counter++] = disableChargeCapacitor1;
						}
						processes_[process_counter++] = enableCapacitor1;
					}
					else{
						if(new_settings_.cp1_charge_){
							processes_[process_counter++] = enableChargeCapacitor1;
							processes_[process_counter++] = enableChargeMosfet1;
						}
						processes_[process_counter++] = disableCapacitor1;
					}
				}
				if(change_cp2){
					if(new_new_settings_.cp2_){
						if(new_settings_.cp2_charge_){
							processes_[process_counter++] = disableChargeMosfet2;
							processes_[process_counter++] = disableChargeCapacitor2;
						}
						processes_[process_counter++] = enableCapacitor2;
					}
					else{
						if(new_settings_.cp2_charge_){
							processes_[process_counter++] = enableChargeCapacitor2;
							processes_[process_counter++] = enableChargeMosfet2;
						}
						processes_[process_counter++] = disableCapacitor2;
					}
				}
				processes_[process_counter++] = enableMosfet;
			}
			bool change_cp1_charge = (new_settings_.cp1_charge_ ^ current_settings_.cp1_charge_ || new_settings_.cp1_	^ current_settings_.cp1_ )&& new_settings_.cp1_charge_ ^ new_settings_.cp1_;
			bool change_cp2_charge = (new_settings_.cp2_charge_ ^ current_settings_.cp2_charge_ || new_settings_.cp2_	^ current_settings_.cp2_ )&& new_settings_.cp2_charge_ ^ new_settings_.cp2_;
			if(change_cp1_charge || change_cp2_charge){ // change
				processes_[process_counter++] = disableMosfet;
				if(change_cp1_charge){
					processes_[process_counter++] = (new_settings_.cp1_charge_ ?	enableChargeCapacitor1 : disableChargeCapacitor1);
				}
				if(change_cp2_charge){
					processes_[process_counter++] = (new_settings_.cp2_charge_ ?	enableChargeCapacitor2 : disableChargeCapacitor2);
				}
				processes_[process_counter++] = enableMosfet;
			}
		}
		else{ // on or off
			processes_[process_counter++] = disableMosfet;
			if(current_settings_.mode_ == ON){
				processes_[process_counter++] = disableRelay;
			}
			if(new_settings_.cp1_)
			{
				if(current_settings_.cp1_charge_){
					processes_[process_counter++] = disableChargeMosfet1;
					processes_[process_counter++] = disableChargeCapacitor1;
				}
				processes_[process_counter++] = enableCapacitor1;
			}
			else if(new_settings_.cp1_charge_){
				processes_[process_counter++] = enableChargeCapacitor1;
				processes_[process_counter++] = enableChargeMosfet1;
			}
			if(new_settings_.cp2_)
			{
				if(current_settings_.cp2_charge_){
					processes_[process_counter++] = disableChargeMosfet2;
					processes_[process_counter++] = disableChargeCapacitor2;

				}
				processes_[process_counter++] = enableCapacitor2;
			}
			else if(new_settings_.cp2_charge_){
				processes_[process_counter++] = enableChargeCapacitor2;
				processes_[process_counter++] = enableChargeMosfet2;
			}
			processes_[process_counter++] = enableMosfet;
		}
	}
	// ***********************************
	processes_[process_counter++] = END;
	process_ptr_ = &processes_[1];
	counter_ = 10;
}

void Engine::processing()
{
	if(*process_ptr_ == enableRelay){
		motor_vcc_.clear();
	}
	else if(*process_ptr_ == disableRelay){
		motor_vcc_.set();
	}
	else if(*process_ptr_ ==  enableMosfet){
		processPWM(new_settings_.pwm_);
	}
	else if(*process_ptr_ == disableMosfet){
		processPWM(0);
	}

	else if(*process_ptr_ == enableCapacitor1){
		cp1_.clear();
	}
	else if(*process_ptr_ == enableCapacitor2)
	{
		cp2_.clear();
	}

	else if(*process_ptr_ == disableCapacitor1){
		cp1_.set();
	}
	else if(*process_ptr_ == disableCapacitor2)
	{
		cp2_.set();
	}
	else if(*process_ptr_ == disableChargeCapacitor1){
		cp1_charge_.set();
	}
	else if(*process_ptr_ == disableChargeCapacitor2)
	{
		cp2_charge_.set();
	}
	else if(*process_ptr_ == enableChargeCapacitor1){
		cp1_charge_.clear();
	}
	else if(*process_ptr_ == enableChargeCapacitor2)
	{
		cp2_charge_.clear();
	}
	else if(*process_ptr_ == enableChargeMosfet1)
	{
		processPwmCp1(new_settings_.cp1_charge_pwm_);
	}
	else if(*process_ptr_ == enableChargeMosfet2)
	{
		processPwmCp2(new_settings_.cp2_charge_pwm_);
	}
	else	if(*process_ptr_ == disableChargeMosfet1)
	{
		processPwmCp1(0);
	}
	else if(*process_ptr_ == disableChargeMosfet2)
	{
		processPwmCp2(0);
	}

	else if(*process_ptr_ == END)
	{
		for(byte i = 0; i < 12; i++)
		{
			processes_[i] = END;
		}
		current_settings_ = new_settings_;
		process_ptr_ = &processes_[0];
		if(new_new_used_){
			new_settings_ = new_new_settings_;
			startProcess();
			in_process_ = true;
			new_new_used_ = false;
			return;
		}
		else{
			in_process_ = false;
		}
		return;
	}
	counter_ = 0xFF;
	++process_ptr_;
}

void Engine::initPWM()
{
	TCCR1A|=(1<<WGM10);
	TCCR1A |= (1 << COM1A1);
	TCCR1B |= (1 << CS10)|(1 << CS10);
	OCR1A = 0xFF; // out at OC1A

	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	TCCR0B |= (1 << CS01);
	OCR0B = 0xFF;
	OCR0A = 0xFF;
	sei();
}

void Engine::processPWM(byte pwm)
{
	OCR1A = 0xFF - pwm;
}

void Engine::processPwmCp1(byte pwm)
{
	OCR0B = 0xFF - pwm;
}

void Engine::processPwmCp2(byte pwm)
{
	OCR0A = 0xFF - pwm;
}

void Engine::setPWM(byte pwm)
{
	if(!in_process_){
		current_settings_.pwm_ = pwm;
		if(current_settings_.mode_ == ON || current_settings_.mode_ == CAPACITOR){
			processPWM(current_settings_.pwm_);
		}
	}
	else{
		if(!new_new_used_){
			new_new_settings_ = new_settings_;
			new_new_used_ = true;
		}
		new_new_settings_.pwm_ = pwm;
	}
}

byte Engine::getPWM()
{
	if(in_process_){
		if(new_new_used_){
			return new_new_settings_.pwm_;
		}
		return new_settings_.pwm_;
	}
	return current_settings_.pwm_;
}

void Engine::setMode(Engine::EngineMode mode)
{
	//	if(mode == OFF){
	//		bitSet(DDRB,0);
	//		bitSet(PORTB,0);
	//	}
	if(in_process_){
		new_new_settings_.mode_ = mode;
		if(!new_new_used_){
			new_new_settings_.cp1_ = new_settings_.cp1_;
			new_new_settings_.cp2_ = new_settings_.cp2_;
			new_new_used_ = true;
		}
	}
	else{
		in_process_ = true;
		new_settings_.mode_ = mode;
		new_settings_.pwm_ = current_settings_.pwm_;
		processes_[0] = START;
		process_ptr_ = &processes_[0];
	}
}

Engine::EngineMode Engine::getMode()
{
	if(in_process_){
		if(new_new_used_){
			return new_new_settings_.mode_;
		}
		return new_settings_.mode_;
	}
	return current_settings_.mode_;
}


void Engine::setCP1(bool state)
{
	// please set before set mode to capacitor!
	if(in_process_){
		if(!new_new_used_){
			new_new_settings_ = new_settings_;
			new_new_used_ = true;
		}
		new_new_settings_.cp1_ = state;
	}
	else{
		in_process_ = true;
		new_settings_.cp1_ = state;
		processes_[0] = START;
		process_ptr_ = &processes_[0];
	}
}

void Engine::setCP2(bool state)
{
	// please set before set mode to capacitor!
	if(in_process_){
		if(!new_new_used_){
			new_new_settings_ = new_settings_;
			new_new_used_ = true;
		}
		new_new_settings_.cp2_ = state;
	}
	else{
		in_process_ = true;
		new_settings_.cp2_ = state;
		processes_[0] = START;
		process_ptr_ = &processes_[0];
	}
}

void Engine::setCP1Charge(bool state)
{
	if(in_process_){
		if(!new_new_used_){
			new_new_settings_ = new_settings_;
			new_new_used_ = true;
		}
		new_new_settings_.cp1_charge_ = state;
	}
	else{
		in_process_ = true;
		new_settings_.cp1_charge_ = state;
		processes_[0] = START;
		process_ptr_ = &processes_[0];
	}
}

void Engine::setCP2Charge(bool state)
{
	if(in_process_){
		if(!new_new_used_){
			new_new_settings_ = new_settings_;
			new_new_used_ = true;
		}
		new_new_settings_.cp2_charge_ = state;
	}
	else{
		in_process_ = true;
		new_settings_.cp2_charge_ = state;
		processes_[0] = START;
		process_ptr_ = &processes_[0];
	}
}

void Engine::setCP1ChargePWM(byte pwm)
{
	if(!in_process_){
		current_settings_.cp1_charge_pwm_ = pwm;
		if(current_settings_.cp1_charge_ && ! current_settings_.cp1_){
			processPwmCp1(current_settings_.cp1_charge_pwm_ = pwm);
		}
	}
	else{
		if(!new_new_used_){
			new_new_settings_ = new_settings_;
			new_new_used_ = true;
		}
		new_new_settings_.cp1_charge_pwm_ = pwm;
	}
}

void Engine::setCP2ChargePWM(byte pwm)
{
	if(!in_process_){
		current_settings_.cp2_charge_pwm_ = pwm;
		if(current_settings_.cp2_charge_ && ! current_settings_.cp2_){
			processPwmCp2(current_settings_.cp2_charge_pwm_ = pwm);
		}
	}
	else{
		if(!new_new_used_){
			new_new_settings_ = new_settings_;
			new_new_used_ = true;
		}
		new_new_settings_.cp2_charge_pwm_ = pwm;
	}
}
