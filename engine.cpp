#include "engine.h"

Engine::Engine()
{
}

void Engine::init(PortPin motor_vcc, PortPin motor_pwm, PortPin charge_relay, PortPin charge_mosfet)
{
	// charge mosfet pwm pin declaration
	motor_vcc_ = motor_vcc;
	motor_pwm_ = motor_pwm;

	charge_relay_ = charge_relay;
	charge_mosfet_ = charge_mosfet;

	// init mosfet pins
	motor_pwm_.clear();
	motor_pwm_.output();

	charge_mosfet_.clear();
	charge_mosfet_.output();


	// init relay pins
	motor_vcc_.set();
	motor_vcc_.output();

	// init Capacitors Pins
	charge_relay_.set();
	charge_relay_.output();

	initPWM();
	processPWM(0);
	processChargePwm(130);
}

void Engine::process()
{
	if (in_process_)
	{
		if (*process_ptr_ == START)
		{
			initProcess();
		}
		else
		{
			if (counter_ && --counter_ == 0)
			{
				processing();
			}
		}
	}
}

void Engine::initProcess()
{
	byte process_counter = 1;

	// ********************************
	if (new_settings_.mode_ == ON)
	{
		// if before mode was capacitor, enable this function.
		if (current_settings_.mode_ == CAPACITOR) // mode before was capacitor
		{
			processes_[process_counter++] = disableMosfet;
			processes_[process_counter++] = enableRelay;
			processes_[process_counter++] = disableCapacitor;
		}

		// charging changed or enabled
		if (current_settings_.charge_ != new_settings_.charge_ || current_settings_.mode_ == OFF)
		{
			processes_[process_counter++] = (new_settings_.charge_ ? enableChargeMosfet : disableChargeMosfet);
		}

		if (current_settings_.mode_ != ON)
		{
			processes_[process_counter++] = enableRelay;
			processes_[process_counter++] = enableMosfet;
		}
	}
	// ********************************
	else if (new_settings_.mode_ == OFF)
	{
		if (current_settings_.mode_ != OFF)
		{
			processes_[process_counter++] = disableMosfet;
			processes_[process_counter++] = disableRelay;
			if (current_settings_.mode_ == CAPACITOR)
			{
				processes_[process_counter++] = disableCapacitor;
			}
		}
		// charge capacitor settings
		if (current_settings_.charge_ != new_settings_.charge_)
		{
			processes_[process_counter++] = (new_new_settings_.charge_ ? enableChargeMosfet : disableChargeMosfet);
		}
	}

	// *******************************
	else if (new_settings_.mode_ == CAPACITOR)
	{
		if (current_settings_.mode_ != CAPACITOR)
		{ // on or off
			processes_[process_counter++] = disableMosfet;

			processes_[process_counter++] = disableChargeMosfet;
			processes_[process_counter++] = enableCapacitor;
			if (current_settings_.mode_ == ON)
			{
				processes_[process_counter++] = disableRelay;
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
	if (*process_ptr_ == enableRelay)
	{
		motor_vcc_.clear();
	}
	else if (*process_ptr_ == disableRelay)
	{
		motor_vcc_.set();
	}
	else if (*process_ptr_ == enableMosfet)
	{
		processPWM(new_settings_.pwm_);
	}
	else if (*process_ptr_ == disableMosfet)
	{
		processPWM(0);
	}

	else if (*process_ptr_ == enableCapacitor)
	{
		charge_relay_.clear();
	}

	else if (*process_ptr_ == disableCapacitor)
	{
		charge_relay_.set();
	}

	else if (*process_ptr_ == disableChargeMosfet)
	{
		processChargePwm(0);
	}

	else if (*process_ptr_ == END)
	{
		for (byte i = 0; i < 12; i++)
		{
			processes_[i] = END;
		}
		current_settings_ = new_settings_;
		process_ptr_ = &processes_[0];
		if (new_new_used_)
		{
			new_settings_ = new_new_settings_;
			initProcess();
			in_process_ = true;
			new_new_used_ = false;
			return;
		}
		else
		{
			in_process_ = false;
		}
		return;
	}
	counter_ = 0xFF;
	++process_ptr_;
}

void Engine::initPWM()
{
	TCCR1A |= (1 << WGM10);
	TCCR1A |= (1 << COM1A1);
	TCCR1B |= (1 << CS11) | (1 << CS10); // clock divide 64
	OCR1A = 0xFF;                        // out at OC1A



//	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
//	TCCR0A |= (1 << WGM01) | (1 << WGM00);
//	TCCR0B |= (1 << CS01) | (1 << CS00);

//	TCCR0B = (1 << WGM02) | (0 << CS01) | (0 << CS00);

	TCCR0A |= (1 << WGM00);
	bitSet(TCCR0A, COM0B1);
	TCCR0B |= (1 << CS01) | (1 << CS00); // clock divide 64

	OCR0B = 0x33;



}

void Engine::processPWM(byte pwm)
{
	OCR1A = 0xFF - pwm;
}

void Engine::processChargePwm(byte pwm)
{
//	OCR0B = 0xFF - pwm;
}

// void Engine::processPwmCp2(byte pwm)
//{
//	OCR0A = 0xFF - pwm;
//}

void Engine::setPWM(byte pwm)
{
	if (!in_process_)
	{
		current_settings_.pwm_ = pwm;
		if (current_settings_.mode_ == ON || current_settings_.mode_ == CAPACITOR)
		{
			processPWM(current_settings_.pwm_);
		}
	}
	else
	{
		if (!new_new_used_)
		{
			new_new_settings_ = new_settings_;
			new_new_used_ = true;
		}
		new_new_settings_.pwm_ = pwm;
	}
}

byte Engine::getPWM()
{
	if (in_process_)
	{
		if (new_new_used_)
		{
			return new_new_settings_.pwm_;
		}
		return new_settings_.pwm_;
	}
	return current_settings_.pwm_;
}

void Engine::setMode(Engine::EngineMode mode)
{
	if (in_process_)
	{
		if (!new_new_used_)
		{
			new_new_settings_ = new_settings_;
			new_new_used_ = true;
		}
		new_new_settings_.mode_ = mode;
	}
	else
	{
		in_process_ = true;
		new_settings_.mode_ = mode;
		new_settings_.pwm_ = current_settings_.pwm_;
		processes_[0] = START;
		process_ptr_ = &processes_[0];
	}
}

Engine::EngineMode Engine::getMode()
{
	if (in_process_)
	{
		if (new_new_used_)
		{
			return new_new_settings_.mode_;
		}
		return new_settings_.mode_;
	}
	return current_settings_.mode_;
}

// void Engine::setCP1(bool state)
//{
//	// please set before set mode to capacitor!
//	if (in_process_)
//	{
//		if (!new_new_used_)
//		{
//			new_new_settings_ = new_settings_;
//			new_new_used_ = true;
//		}
//		new_new_settings_.cp1_ = state;
//	}
//	else
//	{
//		in_process_ = true;
//		new_settings_.cp1_ = state;
//		processes_[0] = START;
//		process_ptr_ = &processes_[0];
//	}
//}

// void Engine::setCP2(bool state)
//{
//	// please set before set mode to capacitor!
//	if (in_process_)
//	{
//		if (!new_new_used_)
//		{
//			new_new_settings_ = new_settings_;
//			new_new_used_ = true;
//		}
//		new_new_settings_.cp2_ = state;
//	}
//	else
//	{
//		in_process_ = true;
//		new_settings_.cp2_ = state;
//		processes_[0] = START;
//		process_ptr_ = &processes_[0];
//	}
//}

void Engine::setCharge(bool state)
{
	if (in_process_)
	{
		if (!new_new_used_)
		{
			new_new_settings_ = new_settings_;
			new_new_used_ = true;
		}
		new_new_settings_.charge_ = state;
	}
	else
	{
		in_process_ = true;
		new_settings_.charge_ = state;
		processes_[0] = START;
		process_ptr_ = &processes_[0];
	}
}

void Engine::setChargePWM(byte pwm)
{
	if (!in_process_)
	{
		current_settings_.charge_pwm_ = pwm;
		if (current_settings_.mode_ != CAPACITOR)
		{
			processChargePwm(current_settings_.charge_pwm_);
		}
	}
	else
	{
		if (!new_new_used_)
		{
			new_new_settings_ = new_settings_;
			new_new_used_ = true;
		}
		new_new_settings_.charge_pwm_ = pwm;
	}
}
