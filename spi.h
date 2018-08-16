#pragma once
#include "avr.h"

class SPI{
public:
	SPI(byte len);
	bool byteFinished();
	byte * getData();
	byte len(){return len_;}
	void transmitt(byte * data);

private:
	const byte len_;
	byte * buffer_;
	byte current_byte_ = 0;
	void initSPI();

};
