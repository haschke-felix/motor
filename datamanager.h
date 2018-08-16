#pragma once
#include "avr.h"
#include "control.h"
#include "spi.h"
class DataManager
{
public:
	DataManager(SPI * spi, int taste_rate = 0xFFFF);
	void init();
	void process();
	void transmissionFinished();


////	void process();
////	void evaluate();

private:

	SPI * spi_;
	void newTransmission();
	int byteArrayToInt(byte * ptr);
	void intToByteArray(int val, byte &b1, byte &b2);
	void intIntoArray(int val, byte * ptr);
	struct Receive{
		byte cp1_discharge;
		byte cp1_charge;
		byte cp1_charge_pwm;
		byte cp2_discharge;
		byte cp2_charge;
		byte cp2_charge_pwm;
		byte motor; // on or off
	} receive;

	struct Tranmitt{
		int v;
		int rpm_is;
		int rpm_should;
		int motor_voltage_is;
		int motor_voltage_should;
		int motor_pwm;
		int cp1_voltage;
		int cp2_voltage;
	} transmitt;

	Control control_;
	int counter_ = 1;
	const int taste_rate_;
};



