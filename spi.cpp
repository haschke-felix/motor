#include"spi.h"

byte current_byte_;

SPI::SPI(byte len) : len_(len)
{
	buffer_ = static_cast<byte*>(malloc(len_));
	initSPI();
}

bool SPI::byteFinished()
{
	byte input = SPDR;
	current_byte_++;
	if(current_byte_ + 1 == len_)
		SPDR = 0;
	else
		SPDR = buffer_[current_byte_];
	buffer_[current_byte_-1] = input;
	if(current_byte_ == len_){
		current_byte_ = 0;
		return true;
	}
	return false;
}


byte *SPI::getData()
{
	return buffer_;
}

void SPI::transmitt(byte *data)
{
	current_byte_ = 0;
	for(int i = 0; i < len_; i++){
		buffer_[i] = data[i];
	}
	SPDR = buffer_[0];
	return;
}
void SPI::initSPI(){
	DDRB=(1<<PINB4);               //MISO as OUTPUT
	SPCR=(1<<SPE)|(1<<SPIE);       //Enable SPI && interrupt enable bit
	SPDR=0;
	sei(); // interrupt enable
}
