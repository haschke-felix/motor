#include "uart.h"


void UART::init(void (*transmitt_finished)(), void (*receive_finished)())
{
	transmitt_finished_ = transmitt_finished;
	receive_finished_ = receive_finished;
	cli();
	UBRR0H = (UBRR0_VALUE >> 8); // baud rate (F_CPU / (16 * Baud))
	UBRR0L = UBRR0_VALUE;

	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << TXCIE0) | (1 << RXCIE0); // enable
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); // 8 bit

	sei();
}

void UART::transmittString(char * string, byte length)
{
	transmitt_bytes_ = length;
	transmitt_count_ = 1;
	for(int i = 0; i < length; i++){
		transmitt[i] = string[i];
	}
	UDR0 = transmitt[0];
}

bool UART::print(const char *string, bool ln){
	if(str_direkt_)
		return false;
	str_direkt_ = true;
	UCSR0B &= ~(1 << TXCIE0); // disable tx vector
	while(true){
		UDR0 = *string;
		while(!(UCSR0A & (1 << UDRE0)));
		string++;
		if(*(string) == '\0'){
			if(ln){
				UDR0 = '\n';
				while(!(UCSR0A & (1 << UDRE0)));
			}
			break;
		}
	}
	UCSR0B |= (1 << TXCIE0); // enable tx vector
	str_direkt_ = false;
	return true;
}

bool UART::print(char ch,bool ln)
{
	char string[2] = {ch};
	const char * str = string;
	return print(str,ln);
}

bool UART::print(int i, bool ln)
{
	char str[8];
	return print(formatInt(str,8,i),ln);
}

bool UART::print(long i, bool ln)
{
	char str[13];
	return print(formatInt(str,13,i),ln);
}

char* UART::formatInt(char* digits, byte size, long value)
{
	if (size < 3) return digits;

	digits[--size] = '\0'; // terminating '\0'
	digits[--size] = '0'; // zero display if value == 0

	bool neg = false;
	if (value < 0) {
		neg = true;
		value = -value;
	} else if (value == 0)
		--size;

	for (; size != 0 && value != 0; --size) {
		digits[size] = '0' + (value % 10);
		value = value / 10;
	}

	if (neg) digits[size] = '-';
	else ++size;

	return digits + size;
}


char *UART::formatInt(char *digits, byte size, int value)
{
	if (size < 3) return digits;

	digits[--size] = '\0'; // terminating '\0'
	digits[--size] = '0'; // zero display if value == 0

	bool neg = false;
	if (value < 0) {
		neg = true;
		value = -value;
	} else if (value == 0)
		--size;

	for (; size != 0 && value != 0; --size) {
		digits[size] = '0' + (value % 10);
		value = value / 10;
	}

	if (neg) digits[size] = '-';
	else ++size;

	return digits + size;
}

const char * UART::getReceive()
{
	return receive;
}


byte UART::getReceiveLength(){
	return received_bytes_;
}

void UART::ISR_TX()
{
	if(transmitt_count_ == transmitt_bytes_){
		transmitt_count_ = 0;
		transmitt_finished_();
		return;
	}
	UDR0 = transmitt[transmitt_count_];
	transmitt_count_++;
}

void UART::ISR_RX()
{
	receive[receive_count_] = UDR0;
	if(receive[receive_count_] == '\n' || receive[receive_count_] == '\0'){
		received_bytes_ = receive_count_;
		receive_count_ = 0;
		receive_finished_();
		return;
	}
	receive_count_++;
	return;
}
