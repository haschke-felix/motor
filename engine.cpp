#include "engine.h"

Engine::Engine()
{

}

void Engine::init(volatile byte *port_motor_vcc, volatile byte *ddr_motor_vcc, byte pin_motor_vcc,
                  volatile byte *port_motor_pwm, volatile byte *ddr_motor_pwm, byte pin_motor_pwm,
                  volatile byte *tccr_motor_vcc, volatile byte *ocr_h_motor_pwm, volatile byte *ocr_l_motor_pwm)
{
	port_motor_pwm_ = port_motor_pwm;
	ddr_motor_pwm_ = ddr_motor_pwm;
	pin_motor_pwm_ = pin_motor_pwm;

	// charge mosfet pwm pin declaration
	port_motor_vcc_ = port_motor_vcc;
	ddr_motor_vcc_ = ddr_motor_vcc;
	pin_motor_vcc_ = pin_motor_vcc;

	tccr_motor_vcc_ = tccr_motor_vcc;
	ocr_h_motor_vcc_ = ocr_h_motor_pwm;
	ocr_l_mosfet_ = ocr_l_motor_pwm;

	// init mosfet pins
	bitSet(*port_motor_pwm_,pin_motor_pwm_);
	bitSet(*ddr_motor_pwm_,pin_motor_pwm_);

	// init relay pins
	bitSet(*port_motor_vcc_,pin_motor_vcc_);
	bitSet(*ddr_motor_vcc_,pin_motor_vcc_);

	initPWM();
}


void Engine::process()
{
	if(next_process_ != Nothing){
		if(counter_ && --counter_ == 0){
			if(mode_ == ON){
				processSwitchOn();
			}
			else if(mode_ == OFF || mode_ == CAPACITOR){
				processSwitchOff();
			}
		}
	}
}

void Engine::initPWM()
{
	//	turn off interrupts (is needed to write OCR1A usw)
	cli();
	bitSet(*ddr_mosfet_,pin_mosfet_);
	bitSet(*tccr_motor_vcc_,0);
	bitSet(*tccr_motor_vcc_,3);
	*tccr_motor_vcc_ |=(1<<CS11); // clock / 8
	sei();
}

void Engine::processPWM(byte pwm)
{
	// writeChargePWM
	*ocr_h_motor_vcc_ |=0x00;
	*ocr_l_mosfet_ |= pwm;
	sei();	//setting PWM-Ports to output is needed
}

void Engine::setPWM(byte pwm)
{
	pwm_ = pwm;
	processPWM(pwm_);
}

void Engine::setMode(Engine::EngineMode mode)
{
	mode_ = mode;
	if(mode_ == CAPACITOR){
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
		counter_ = 100;
	}
}

void Engine::processSwitchOff()
{
	if(next_process_ == clearMosfet){
		processPWM(0);
		next_process_ = clearRelay;
		counter_ = 100;
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
		counter_ = 100;
	}
}

void Engine::processSwitchOn()
{
	if(next_process_ == setRelay){
		bitClear(*port_motor_vcc_,pin_motor_vcc_);
		next_process_ = setMosfet;
		counter_ = 100;
	}
	else if(next_process_ == setMosfet){
		processPWM(pwm_);
		next_process_ = Nothing;
	}
}
