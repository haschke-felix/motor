#pragma once
#include "avr.h"
#include "control.h"
#include "usb.h"
#include "measurement.h"

class DataManager
{
public:
	DataManager(unsigned int update_count = 0);
	void process();
	void receivedValues(const byte *data);

private:
	void transmitt();

	struct Receive
	{
		byte max_boost_pwm;
	} receive_;

	Control control_;
	unsigned int counter_ = 0;
	/// number of iterations before updating send values
	const unsigned int update_count_;
};
