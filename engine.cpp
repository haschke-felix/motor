#include "engine.h"

Engine::Engine()
{
}

Engine::~Engine()
{
	process_list_.removeAll();
}

void Engine::init(PortPin motor_vcc, PortPin motor_pwm, PortPin capacitor_relay, PortPin charge_relay,
						PortPin charge_mosfet)
{
	// charge mosfet pwm pin declaration
	motor_vcc_ = motor_vcc;
	motor_pwm_ = motor_pwm;

	capacitor_relay_ = capacitor_relay;
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
	capacitor_relay_.set();
	capacitor_relay_.output();

	charge_relay_.set();
	charge_relay_.output();

	initPWM();
	processPWM(0);
	processChargePwm(0);
}

void Engine::process()
{
	if (in_process_)
	{
		if (process_list_.first() == START && !process_list_.isEmpty())
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
	// ********************************
	process_list_.removeAll();
	if (new_settings_.mode_ == ON)
	{
		// if before mode was capacitor, enable this function.
		if (current_settings_.mode_ == CAPACITOR) // mode before was capacitor
		{
			process_list_ << disableMosfet;
			process_list_ << enableRelay;
			process_list_ << disableCapacitor;
		}

		// charging changed or enabled
		if (current_settings_.charge_ != new_settings_.charge_ ||
			 current_settings_.charge_pwm_ != new_settings_.charge_pwm_ || current_settings_.mode_ == CAPACITOR)
		{
			process_list_ << (new_settings_.charge_ ? enableChargeCapacitor : disableChargeCapacitor);
			process_list_ << (new_settings_.charge_ ? enableChargeMosfet : disableChargeMosfet);
		}

		if (current_settings_.mode_ != ON)
		{
			process_list_ << enableRelay;
		}
		process_list_ << enableMosfet; // updates also the pwm
	}

	// ********************************
	else if (new_settings_.mode_ == OFF)
	{
		if (current_settings_.mode_ != OFF)
		{
			process_list_ << disableMosfet;
			process_list_ << disableChargeCapacitor;
			if (current_settings_.mode_ == CAPACITOR)
			{
				process_list_ << disableCapacitor;
			}
			process_list_ << disableRelay;
		}
		// charge not possible
	}

	// *******************************
	else if (new_settings_.mode_ == CAPACITOR)
	{
		if (current_settings_.mode_ != CAPACITOR)
		{ // on or off
			process_list_ << disableMosfet;

			process_list_ << disableChargeMosfet;
			process_list_ << disableChargeCapacitor;
			process_list_ << enableCapacitor;
			if (current_settings_.mode_ == ON)
			{
				process_list_ << disableRelay;
			}
			process_list_ << enableMosfet;
		}
	}
	// ***********************************
	process_list_ << END;
	counter_ = 10;
}

void Engine::processing()
{
	Process step = process_list_.first();
	process_list_.removeFirst();
	if (step == enableRelay)
	{
		motor_vcc_.clear();
	}
	else if (step == disableRelay)
	{
		motor_vcc_.set();
	}
	else if (step == enableMosfet)
	{
		processPWM(new_settings_.pwm_);
	}
	else if (step == disableMosfet)
	{
		processPWM(0);
	}

	else if (step == enableCapacitor)
	{
		capacitor_relay_.clear();
	}

	else if (step == disableCapacitor)
	{
		capacitor_relay_.set();
	}

	else if (step == enableChargeCapacitor)
	{
		charge_relay_.clear();
	}

	else if (step == disableChargeCapacitor)
	{
		charge_relay_.set();
	}

	else if (step == disableChargeMosfet)
	{
		processChargePwm(0);
	}

	else if (step == enableChargeMosfet)
	{
		processChargePwm(new_settings_.charge_pwm_);
	}

	if (step == END || process_list_.isEmpty())
	{
		process_list_.removeAll();
		current_settings_ = new_settings_;
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
}

void Engine::initPWM()
{
	TCCR1A |= (1 << WGM10);
	TCCR1A |= (1 << COM1A1);
	TCCR1B |= (1 << CS11) | (1 << CS10); // clock divide 64
	OCR1A = 0xFF;                        // out at OC1A

	TCCR0A |= (1 << WGM00);
	bitSet(TCCR0A, COM0B1);
	TCCR0B |= (1 << CS01) | (1 << CS00); // clock divide 64

	OCR0B = 0xFF;
}

void Engine::processPWM(const byte pwm)
{
	OCR1A = 0xFF - pwm;
}

void Engine::processChargePwm(const byte pwm)
{
	OCR0B = 0xFF - pwm;
}

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
		process_list_ << START;
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
		process_list_ << START;
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
