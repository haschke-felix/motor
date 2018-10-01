#pragma once
#include "avr.h"
#include "control.h"
#include "spi.h"
class DataManager
{
public:
    DataManager(SPI *spi, unsigned int update_count = 0);
	void init();
	void process();
	void receivedValues(const byte *data);

private:
	int  byteArrayToInt(byte * ptr);
	void intToByteArray(int val, byte &b1, byte &b2);
	void intIntoArray(int val, byte * ptr);

	void updateTransmitData();
	void evaluate();

	SPI * spi_;
	struct Receive{
		byte max_boost_pwm;
	} receive_;

    Control control_;
	unsigned int counter_ = 0;
	/// number of iterations before updating send values
	const unsigned int update_count_;
};



