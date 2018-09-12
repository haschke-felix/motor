#pragma once
#include "avr.h"

class SPI{
public:
	SPI(byte send, byte recv);
	/// method triggered when transmission of single byte was finished
	const byte* byteFinished();
	/// queue new data packet to answer
	void setSendBuffer(const byte *data);
	byte sendBufferSize() const {return num_send_;}
	byte recvBufferSize() const {return num_recv_;}

private:
	const byte num_send_, num_recv_;
	byte *send_buffer_, *pending_buffer_, *recv_buffer_;
	byte current_byte_ = 0;
	bool pending_ = false; // true if should switch buffers after next transmit cycle
	void initSPI();
};
