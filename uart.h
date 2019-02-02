#pragma once
#include "avr.h"
#include "string.h"

#define BAUD 9600UL
#define UBRR0_VALUE ((F_CPU/(16*BAUD)) - 1)

class UART
{
public:
	void init(void (*transmitt_finished)(void), void(*receive_finished)(void));
	void transmittString(char *string, byte length);
	bool print(const char *string, bool ln);
	bool print(char ch, bool ln);
	bool print(int i, bool ln);
	bool print(long i, bool ln);


	bool print(char *string){return print(string,false);}
	bool print(const char *string){return print(string,false);}
	bool print(char ch) {return print(ch,false);}
	bool print(int i){print(i,false);}
	bool print(long i){print(i,false);}

	bool println(const char *string){return print(string,true);}
	bool println(char *string){return print(string,true);}
	bool println(char ch){return print(ch,true);}
	bool println(int i){print(i,true);}
	bool println(long i){print(i,true);}


	const char *getReceive();
	void ISR_TX();
	void ISR_RX();

	byte getReceiveLength();

	char *formatInt(char *digits, byte size, int value);
	char *formatInt(char *digits, byte size, long value);



private:
	char receive[256], transmitt[256];
	void(*receive_finished_)(void);
	void(*transmitt_finished_)(void);
	byte receive_count_;
	byte transmitt_count_;
	byte transmitt_bytes_, received_bytes_;
	bool str_direkt_;

};
