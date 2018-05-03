#include "engine.h"

Engine::Engine()
{

}

void Engine::init(volatile byte *port_motor_pwm, volatile byte *ddr_motor_pwm, byte pin_motor_pwm,
                  volatile byte *port_motor_vcc, volatile byte *ddr_motor_vcc, byte pin_motor_vcc)
{
	port_motor_pwm_ = port_motor_pwm;
	ddr_motor_pwm_ = ddr_motor_pwm;
	pin_motor_pwm_ = pin_motor_pwm;
	port_motor_vcc_ = port_motor_vcc;
	ddr_motor_vcc_ = ddr_motor_vcc;
	pin_motor_vcc_ = pin_motor_vcc;
}

void Engine::process()
{
	if(next_process_ != Nothing){
		if(mode_ == ON){
			processSwitchOn();
		}
		else if(mode_ == OFF || mode_ == Capacitor){
			processSwitchOff();
		}
	}
}

void Engine::processPWM(byte pwm)
{
	1<<8;
}

void Engine::setPWM(byte pwm)
{

}

void Engine::setMode(Engine::EngineMode mode)
{
	mode_ = mode;
	if(mode_ == Capacitor){
		switchOff();
	}
	else if(mode_ == ON){
		switchOn();
	}
	else if(mode_ == OFF){
		switchOff();
	}


}

void Engine::switchOff()
{
	if(mode_ != OFF && next_process_ == Nothing){
		mode_ = OFF;
		next_process_ = clearMosfet;
	}
}

void Engine::processSwitchOff()
{
	if(next_process_ == clearMosfet){
		processPWM(0);
		next_process_ = clearRelay;
	}
	else if(next_process_ == clearRelay){
		bitSet(*port_motor_vcc_,pin_motor_vcc_);
		next_process_ = Nothing;
	}
}

void Engine::switchOn()
{
	if(mode_ != ON && next_process_ == Nothing){
		mode_ = ON;
		next_process_ = setRelay;
	}
}

void Engine::processSwitchOn()
{
	if(next_process_ == setRelay){
		bitClear(*port_motor_vcc_,pin_motor_vcc_);
		next_process_ = setMosfet;
	}
	else if(next_process_ == setMosfet){
		processPWM(pwm_);
		next_process_ = Nothing;
	}
}

void Engine::updatePWM()
{

}
