#include "spi.h"

SPI::SPI(byte len) : len_(len)
{
	buffer_ = static_cast<byte*>(malloc(len_));
	initSPI();
}

bool SPI::byteFinished()
{
	byte input = SPDR; // received byte
	if(++current_byte_ < len_) // transmit next byte?
		SPDR = buffer_[current_byte_];
	buffer_[current_byte_-1] = input;
	return !transmitting();
}

const byte *SPI::getData() const
{
	return buffer_;
}

void SPI::transmit(const byte *data)
{
	for(int i = 0; i < len_; ++i){
		buffer_[i] = data[i];
	}
	// start new transmission with first byte
	current_byte_ = 0;
	SPDR = buffer_[current_byte_];
	return;
}

void SPI::initSPI(){
	DDRB=(1<<PINB4);               // MISO as OUTPUT
	SPCR=(1<<SPE)|(1<<SPIE);       // Enable SPI && interrupt enable bit
	SPDR=0;
	sei(); // interrupt enable
}
