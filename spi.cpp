#include"spi.h"

byte current_byte_;
volatile byte * receive_buffer_;
volatile byte * send_buffer_;
volatile bool * evaluate_;

void initSPI(volatile byte *receive, volatile byte *send, volatile bool * evaluate){
	evaluate_ = evaluate;
	receive_buffer_ = receive;
	send_buffer_ = send;
	DDRB=(1<<PINB4);               //MISO as OUTPUT
	SPCR=(1<<SPE)|(1<<SPIE);       //Enable SPI && interrupt enable bit
	SPDR=0;
	sei();
}

ISR(SPI_STC_vect){
	*(receive_buffer_ + current_byte_) = SPDR;
	SPDR = *(send_buffer_ + current_byte_);
	if(current_byte_ == 32){
		current_byte_ = 0;
		*evaluate_ = true;
	}
}
