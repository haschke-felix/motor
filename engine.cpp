#include "engine.h"

Engine::Engine()
{

}

void Engine::init(volatile byte *port_motor_vcc, volatile byte *ddr_motor_vcc, byte pin_motor_vcc,
                  volatile byte *port_motor_pwm, volatile byte *ddr_motor_pwm, byte pin_motor_pwm)
{
	motor_pwm_.port_  = port_motor_pwm;
	motor_pwm_.ddr_ = ddr_motor_pwm;
	motor_pwm_.pin_ = pin_motor_pwm;

	// charge mosfet pwm pin declaration
	motor_vcc_.port_ = port_motor_vcc;
	motor_vcc_.ddr_ = ddr_motor_vcc;
	motor_vcc_.pin_ = pin_motor_vcc;

	// init mosfet pins
	bitSet(*motor_pwm_.port_,motor_pwm_.pin_);
	bitSet(*motor_pwm_.ddr_,motor_pwm_.pin_);

	// init relay pins
	bitSet(*motor_vcc_.port_,motor_vcc_.pin_);
	bitSet(*motor_vcc_.ddr_,motor_vcc_.pin_);


	// only test
	//	bitClear(*motor_vcc_.port_,motor_vcc_.pin_);
	//	current_settings_.mode_ = ON;

	initPWM();
	//	processPWM(55);
	//	bitSet(DDRC,4);
	//	bitSet(PORTC,4);
	//	   while(true);

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
		if(current_settings_.mode_ == OFF){
			processes_[0] == END;
			//					finished();
			return;
		}
		processes_[process_counter++] = disableMosfet;
		if(current_settings_.mode_ == CAPACITOR){
			processes_[process_counter++] = disableCapacitors;
		}
		processes_[process_counter++] = disableRelay;
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
}

void Engine::processing()
{

	if(*process_ptr_ == enableRelay){
		bitClear(*motor_vcc_.port_,motor_vcc_.pin_);
	}
	else if(*process_ptr_ == disableRelay){
		bitSet(*motor_vcc_.port_,motor_vcc_.pin_);
	}
	else if(*process_ptr_ ==  enableMosfet){
		processPWM(new_settings_.pwm_);
	}
	else if(*process_ptr_ == disableMosfet){
		processPWM(0);
	}
	else if(*process_ptr_ == enableCapacitor1){
		bitClear(*cp1_.port_,cp1_.pin_);
	}
	else if(*process_ptr_ == enableCapacitor2){
		bitClear(*cp2_.port_,cp2_.pin_);
	}
	else if(*process_ptr_ == disableCapacitors){
		bitSet(*cp1_.port_,cp1_.pin_);
		bitSet(*cp2_.port_,cp2_.pin_);
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
		}
		else{
			in_process_ = false;
			bitSet(DDRC,4);
			bitToggle(PORTC,4);
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
	OCR1A = 0; // out at OC1A
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

void Engine::setMode(Engine::EngineMode mode)
{

	if(in_process_){
		new_new_settings_.mode_ = mode;
		if(!new_new_used_){
			new_new_used_ = true;
			new_new_settings_.mode_ = new_settings_.mode_;
			new_new_settings_.cp1_ = new_settings_.cp1_;
			new_new_settings_.cp2_ = new_settings_.cp2_;
		}
	}
	else{

		in_process_ = true;
		new_settings_.mode_ = new_new_settings_.mode_ = mode;
		new_settings_.pwm_ = new_new_settings_.pwm_ = current_settings_.pwm_;
		processes_[0] = START;
		process_ptr_ = &processes_[0];
	}
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
