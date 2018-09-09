#pragma once
#include "avr.h"

class SPI{
public:
	SPI(byte len);
	/// method triggered when transmission of single byte was finished
	bool byteFinished();
	bool transmitting() const { return current_byte_ < len_; }
	/// get receive buffer (after transmission finished)
	const byte * getData() const;
	/// start transmission of new data packet
	void transmit(const byte *data);
	byte len() const {return len_;}

private:
	const byte len_;
	byte * buffer_;
	byte current_byte_ = 0xFF;
	void initSPI();
};
