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

private:

	SPI * spi_;
	void newTransmission();
	int byteArrayToInt(byte * ptr);
	void intToByteArray(int val, byte &b1, byte &b2);
	void intIntoArray(int val, byte * ptr);
	void evaluate();
	struct Receive{
		bool cp1_discharge;
		bool cp1_charge;
		byte cp1_charge_pwm;
		bool cp2_discharge;
		bool cp2_charge;
		byte cp2_charge_pwm;
		byte motor; // on or off or capacitor
	} receive_, old_receive_;

	struct Tranmitt{
		int v;
		int rpm_is;
		int rpm_should;
		int motor_voltage_is;
		int motor_voltage_should;
		byte motor_pwm;
		int cp1_voltage;
		int cp2_voltage;
	} transmitt_;

	Control control_;
	int counter_ = 1;
	const int sampling_rate_;
};



