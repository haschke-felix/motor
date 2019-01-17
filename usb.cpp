#include "usb.h"

char USB::receive[MAX_TRANSMITT_LEN], USB::transmitt_[MAX_TRANSMITT_LEN];
void (*USB::receive_finished_)(void);
void (*USB::transmitt_finished_)(void);
byte USB::receive_count_;
byte USB::transmitt_count_;
byte USB::transmitt_bytes_, USB::received_bytes_;
bool USB::str_direkt_;
bool USB::bus_free_ = true;

void USB::USBinit(void (*transmitt_finished)(), void (*receive_finished)())
{
	transmitt_finished_ = transmitt_finished;
	receive_finished_ = receive_finished;
	cli();
	UBRR0H = (UBRR0_VALUE >> 8); // baud rate (F_CPU / (16 * Baud))
	UBRR0L = UBRR0_VALUE;

	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << TXCIE0) | (1 << RXCIE0); // enable
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);                               // 8 bit

	sei();
}

void USB::transmittString(char *string, byte length, bool ln)
{
	if (!bus_free_)
		return;
	bus_free_ = false;
	transmitt_bytes_ = length;
	transmitt_count_ = 1;
	for (int i = 0; i < length; i++)
	{
		transmitt_[i] = string[i];
	}
	if (ln)
	{
		transmitt_bytes_++;
		transmitt_[length] = '\n';
	}
	UDR0 = transmitt_[0];
}

bool USB::print(const char *string, bool ln)
{
	if (str_direkt_)
		return false;
	str_direkt_ = true;
	UCSR0B &= ~(1 << TXCIE0); // disable tx vector
	while (true)
	{
		if (*(string) == '\0')
		{
			if (ln)
			{
				UDR0 = '\n';
				while (!(UCSR0A & (1 << UDRE0)))
					;
			}
			UCSR0B |= (1 << TXCIE0); // enable tx vector
			str_direkt_ = false;
			return true;
		}
		UDR0 = *string;
		while (!(UCSR0A & (1 << UDRE0)))
			;
		string++;
	}
}

bool USB::print(char ch, bool ln)
{
	char string[2] = {ch};
	const char *str = string;
	return print(str, ln);
}

bool USB::print(int i, bool ln)
{
	char str[8];
	return print(formatInt(str, 8, i), ln);
}

bool USB::print(long i, bool ln)
{
	char str[13];
	return print(formatInt(str, 13, i), ln);
}

char *USB::formatInt(char *digits, byte size, long value)
{
	if (size < 3)
		return digits;

	digits[--size] = '\0'; // terminating '\0'
	digits[--size] = '0';  // zero display if value == 0

	bool neg = false;
	if (value < 0)
	{
		neg = true;
		value = -value;
	}
	else if (value == 0)
		--size;

	for (; size != 0 && value != 0; --size)
	{
		digits[size] = '0' + (value % 10);
		value = value / 10;
	}

	if (neg)
		digits[size] = '-';
	else
		++size;

	return digits + size;
}

char *USB::formatInt(char *digits, byte size, int value)
{
	if (size < 3)
		return digits;

	digits[--size] = '\0'; // terminating '\0'
	digits[--size] = '0';  // zero display if value == 0

	bool neg = false;
	if (value < 0)
	{
		neg = true;
		value = -value;
	}
	else if (value == 0)
		--size;

	for (; size != 0 && value != 0; --size)
	{
		digits[size] = '0' + (value % 10);
		value = value / 10;
	}

	if (neg)
		digits[size] = '-';
	else
		++size;

	return digits + size;
}

const char *USB::getReceive()
{
	return receive;
}

byte USB::getReceiveLength()
{
	return received_bytes_;
}

ISR(USART_TX_vect)
{
	USB::ISR_TX();
}

void USB::ISR_TX()
{
	if (transmitt_count_ == transmitt_bytes_ || transmitt_count_ > 254)
	{
		transmitt_count_ = 0;
		bus_free_ = true;
		transmitt_finished_();
		return;
	}
	UDR0 = transmitt_[transmitt_count_];
	if (transmitt_[transmitt_count_] == '\n')
	{
		if (transmitt_count_ + 1 == transmitt_bytes_)
		{                                      // if finished yet
			transmitt_[transmitt_count_] = 'n'; // dont need to decrese because it dont will increased becauseof else
		}
		else
		{
			transmitt_[transmitt_count_] = 'h'; // h for hex
		}
	}
	else
	{
		transmitt_count_++;
	}
}

ISR(USART_RX_vect)
{
	USB::ISR_RX();
}

void USB::ISR_RX()
{
	bus_free_ = false;
	receive[receive_count_] = UDR0;
	if (receive[receive_count_] == '\n')
	{
		receive[receive_count_] = '\0';
		received_bytes_ = receive_count_;
		receive_count_ = 0;
		bus_free_ = true;
		receive_finished_();
		return;
	}
	receive_count_++;
	return;
}
