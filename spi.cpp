#include "spi.h"

#define STATIC_ASSERT(COND,MSG) \
	typedef char static_assertion_##MSG[(COND)?1:-1]

SPI::SPI(byte send, byte recv) : num_send_(send), num_recv_(recv)
{
	STATIC_ASSERT(recv <= send, invalid_buffer_sizes);

	send_buffer_ = static_cast<byte*>(malloc(num_send_));
	pending_buffer_ = static_cast<byte*>(malloc(num_send_));
	recv_buffer_ = static_cast<byte*>(malloc(num_recv_));
	for (byte i = 0; i < num_send_; ++i)
		send_buffer_[i] = 0;

	initSPI();
}

// triggered by SPI interrupt
// return NULL if transmission still active
// return received data after successful transmission
const byte* SPI::byteFinished()
{
	byte received = SPDR; // received byte
	if (current_byte_ == 0) {
		if (received != 0xAC) { // expect 0xAC from master
			// otherwise wait for start byte, send 0xFF
			SPDR = 0xFF;
			return NULL;
		}
	}
	// don't store initial 0xAC byte
	if (current_byte_ > 0 && current_byte_ <= num_recv_)
		recv_buffer_[current_byte_-1] = received;

	if (current_byte_ < num_send_) // transmit next byte?
		SPDR = send_buffer_[current_byte_];
	else
		SPDR = 0;

	if (++current_byte_ < num_send_)
		return NULL; // not yet done

	// transmission finished
	current_byte_ = 0; // start over in next cycle

	if (pending_) { // swap send with pending buffer?
		byte* tmp = send_buffer_;
		send_buffer_ = pending_buffer_;
		pending_buffer_ = tmp;
		pending_ = false;
	}
	return recv_buffer_;
}

void SPI::setSendBuffer(const byte *data)
{
	pending_ = false; // prevent interrupt handler from switching
	for(int i = 0; i < num_send_; i++){
		pending_buffer_[i] = data[i];
	}
	pending_ = true; // trigger buffer switching in interrupt handler
}

void SPI::initSPI(){
	// MISO output
	DDRB |= (1<<PB4);

	// SPI enable interrupts
	SPCR |= (1<<SPE) | (1<<SPIE);
}
