#include "engine.h"

Engine::Engine()
{

}

void Engine::init(PortPin motor_vcc, PortPin motor_pwm, PortPin cp1, PortPin cp2)
{
	// charge mosfet pwm pin declaration
	motor_vcc_ = motor_vcc;
	motor_pwm_ = motor_pwm;

	cp1_ = cp1;
	cp2_ = cp2;

	// init mosfet pins
	bitSet(*motor_pwm_.port,motor_pwm_.pin);
	bitSet(*motor_pwm_.ddr,motor_pwm_.pin);

	// init relay pins
	bitSet(*motor_vcc_.port,motor_vcc_.pin);
	bitSet(*motor_vcc_.ddr,motor_vcc_.pin);

	// init Capacitors Pins
	bitSet(*cp1_.port,cp1_.pin);
	bitSet(*cp1_.ddr,cp1_.pin);

	bitSet(*cp2_.port,cp2_.pin);
	bitSet(*cp2_.ddr,cp2_.pin);

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
	if(new_settings_.mode_ == ON)
	{
		if(current_settings_.mode_ ==  CAPACITOR)
		{
			processes_[process_counter++] = disableMosfet;
			processes_[process_counter++] = disableCapacitors;
		}
		processes_[process_counter++] = enableRelay;
		processes_[process_counter++] = enableMosfet;
	}

	else if(new_settings_.mode_ == OFF)
	{
		if(current_settings_.mode_ != OFF){
			processes_[process_counter++] = disableMosfet;
			if(current_settings_.mode_ == CAPACITOR){
				processes_[process_counter++] = disableCapacitors;
			}
			processes_[process_counter++] = disableRelay;
		}
	}
	else if(new_settings_.mode_ == CAPACITOR)
	{
		if(current_settings_.mode_ == CAPACITOR)
		{
			// check currently setted caps
		}
		else{
			processes_[process_counter++] = disableMosfet;
			if(new_settings_.mode_ == ON){
				processes_[process_counter++] = disableRelay;
			}
			if(new_settings_.cp1_)
			{
				processes_[process_counter++] = enableCapacitor1;
			}
			if(new_settings_.cp2_)
			{
				processes_[process_counter++] = enableCapacitor2;
			}
			processes_[process_counter++] = enableMosfet;
		}
	}
	processes_[process_counter++] = END;
	process_ptr_ = &processes_[1];
	counter_ = 10;
}

void Engine::processing()
{
	if(*process_ptr_ == enableRelay){
		bitClear(*motor_vcc_.port,motor_vcc_.pin);
	}
	else if(*process_ptr_ == disableRelay){
		bitSet(*motor_vcc_.port,motor_vcc_.pin);
	}
	else if(*process_ptr_ ==  enableMosfet){
		processPWM(new_settings_.pwm_);
	}
	else if(*process_ptr_ == disableMosfet){
		processPWM(0);
	}
	else if(*process_ptr_ == enableCapacitor1){
		bitClear(*cp1_.port,cp1_.pin);
	}
	else if(*process_ptr_ == enableCapacitor2){
		bitClear(*cp2_.port,cp2_.pin);
	}
	else if(*process_ptr_ == disableCapacitors){
		bitSet(*cp1_.port,cp1_.pin);
		bitSet(*cp2_.port,cp2_.pin);
	}
	else if(*process_ptr_ == END){
		for(byte i = 0; i < 8; i++){
			processes_[i] = END;
		}

		current_settings_.mode_ = new_settings_.mode_;
		current_settings_.pwm_ = new_settings_.pwm_;
		current_settings_.cp1_ = new_settings_.cp1_;
		current_settings_.cp2_ = new_settings_.cp2_;

		process_ptr_ = &processes_[0];
		if(new_new_used_){
			new_settings_.mode_ = new_new_settings_.mode_;
			new_settings_.pwm_ = new_new_settings_.pwm_;
			new_settings_.cp1_ = new_new_settings_.cp1_;
			new_settings_.cp2_ = new_new_settings_.cp2_;
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
	counter_ = 10;
	++process_ptr_;
}

void Engine::initPWM()
{
	TCCR1A|=(1<<WGM10);
	TCCR1A |= (1 << COM1A1)|(1 << COM1B1)/*|(1 << COM1C1)*/;
	TCCR1B |= (1 << CS10)|(1 << CS10);
	OCR1A = 0xFF; // out at OC1A
	sei();
}

void Engine::processPWM(byte pwm)
{
	OCR1A = 0xFF - pwm;
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
		new_new_settings_.pwm_ = pwm;
		if(!new_new_used_){
			new_new_settings_.mode_ = new_settings_.mode_;
			new_new_settings_.cp1_ = new_settings_.cp1_;
			new_new_settings_.cp2_ = new_settings_.cp2_;
			new_new_used_ = true;
		}
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
	Settings * settings_ptr = &new_settings_;
	if(in_process_)
		settings_ptr = &new_new_settings_;
	settings_ptr->cp1_ = state;
}

void Engine::setCP2(bool state)
{
	// please set before set mode to capacitor!
	Settings * settings_ptr = &new_settings_;
	if(in_process_)
		settings_ptr = &new_new_settings_;
	settings_ptr->cp2_ = state;
}
