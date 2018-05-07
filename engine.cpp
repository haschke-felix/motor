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
	bitClear(*motor_vcc_.port_,motor_vcc_.pin_);
	current_settings_.mode_ = ON;


	initPWM();
	processPWM(0);
}


void Engine::process()
{
	if(in_process_){
		if(counter_ && --counter_ == 0){

		}
	}
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
	}
}

void Engine::setMode(Engine::EngineMode mode)
{

	Settings * settins_ptr = (in_process_ ? &new_new_settings_ : &new_settings_);
	settins_ptr->mode_ = mode;

	if(in_process_){
		byte old_pwm = (in_process_ ? new_settings_.pwm_ : current_settings_.pwm_);
		if(settins_ptr->mode_ == CAPACITOR){
			settins_ptr->pwm_ = old_pwm;
			switchRelayOff();
		}
		else if(settins_ptr->mode_ == ON){
			settins_ptr->pwm_ = old_pwm;
			switchRelayOn();
		}
		else if(settins_ptr->mode_ == OFF){
			settins_ptr->pwm_ = old_pwm;
			switchRelayOff();
		}
	}
}

void Engine::switchRelayOff()
{
//	if(mode_ != OFF && process_ == Nothing){
//		mode_ = OFF;
//		process_ = clearMosfet;
//		counter_ = 100;
//	}
}

void Engine::processSwitchOff()
{
//	if(process_ == clearMosfet){
//		processPWM(0);
//		process_ = clearRelay;
//		counter_ = 100;
//	}
//	else if(process_ == clearRelay){
//		bitSet(*port_motor_vcc_,pin_motor_vcc_);
//		process_ = Nothing;
//	}
}

void Engine::switchRelayOn()
{
//	if(mode_ != ON && process_ == Nothing){
//		mode_ = ON;
//		process_ = setRelay;
//		counter_ = 100;
//	}
}

void Engine::processSwitchOn()
{
//	if(process_ == setRelay){
//		bitClear(*port_motor_vcc_,pin_motor_vcc_);
//		process_ = setMosfet;
//		counter_ = 100;
//	}
//	else if(process_ == setMosfet){
//		processPWM(pwm_);
//		process_ = Nothing;
//	}
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
