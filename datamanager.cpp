#include "datamanager.h"

DataManager::DataManager(unsigned int update_count) : update_count_(update_count)
{
	control_.init();
}

void DataManager::process()
{
	control_.process();
	if (update_count_ && ++counter_ >= update_count_)
	{
		counter_ = 0;
		transmitt();
	}
}

union intByte {
	int i;
	byte b[2];
};

// void DataManager::receivedValues(const byte *data)
//{
//	receive_.max_boost_pwm = data[0];
//	control_.setMaxBoostPWM(receive_.max_boost_pwm);
//}

// int DataManager::byteArrayToInt(byte *ptr)
//{
//	intByte u;
//	u.b[0] = ptr[0];
//	u.b[1] = ptr[1];
//	return u.i;
//}

// void DataManager::intToByteArray(int val, byte &b1, byte &b2)
//{
//	intByte u;
//	u.i = val;
//	b1 = u.b[0];
//	b2 = u.b[1];
//}

// void DataManager::intIntoArray(int val, byte *ptr)
//{
//	intByte u;
//	u.i = val;
//	ptr[0] = u.b[0];
//	ptr[1] = u.b[1];
//}

// void DataManager::evaluate()
//{
//	//	if(receive_.cp1_discharge != old_receive_.cp1_discharge)
//	//		control_.setCP1(receive_.cp1_discharge);
//	//	if(receive_.cp2_discharge != old_receive_.cp2_discharge)
//	//		control_.setCP2(receive_.cp2_discharge);

//	//	if(receive_.cp1_charge != old_receive_.cp1_charge)
//	//		control_.setCP1Charge(receive_.cp1_charge);
//	//	if(receive_.cp2_charge != old_receive_.cp2_charge)
//	//		control_.setCP2Charge(receive_.cp2_charge);

//	//	if(receive_.cp1_charge_pwm != old_receive_.cp1_charge_pwm)
//	//		control_.setCP1ChargePWM(receive_.cp1_charge_pwm);
//	//	if(receive_.cp2_charge_pwm != old_receive_.cp2_charge_pwm)
//	//		control_.setCP2ChargePWM(receive_.cp2_charge_pwm);

//	//	if(receive_.motor != old_receive_.motor)
//	//		control_.setMode(Engine::EngineMode(receive_.motor));
//	//	return;
//}

int DataManager::getADC(byte channel)
{
	if (channel > 7)
		return 0;
	ADMUX &= 0xf0 + channel;
	ADCSRA |= _BV(ADSC);
	while ((ADCSRA & _BV(ADSC)))
		;
	return ADC;
}

void DataManager::transmitt()
{
	USB::print("PWM:");
	USB::println(control_.getPWM());
	USB::print("Boost:");
	USB::println(control_.getMode() == Control::CAPACITOR);
	USB::print("Battery-Voltage:");
	USB::println(getADC(1));
	USB::print("Converter-Voltage:");
	USB::println(getADC(2));
	USB::print("Motor-Voltage:");
	USB::println(getADC(3));
	USB::print("Cp1+-Voltage:");
	USB::println(getADC(4));
	USB::print("Cp1--Voltage:");
	USB::println(getADC(5));
}
