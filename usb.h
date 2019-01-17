#pragma once
#include "avr.h"
#include "string.h"

#define BAUD 9600UL
#define UBRR0_VALUE ((F_CPU/(16*BAUD)) - 1)
#define MAX_TRANSMITT_LEN 255


extern "C" void USART_TX_vect(void) __attribute__ ((signal,__INTR_ATTRS));
extern "C" void USART_RX_vect(void) __attribute__ ((signal,__INTR_ATTRS));

class USB
{
public:
	friend void USART0_TX_vect();
	friend void USART0_RX_vect();


	static void USBinit(void (*transmitt_finished)(void), void(*receive_finished)(void));
	static void transmittString(char *string, byte length, bool ln = true);

	static bool print(const char *string, bool ln);
	static bool print(char ch, bool ln);
	static bool print(int i, bool ln);
	static bool print(long i, bool ln);


	static bool print(char *string){return print(string,false);}
	static bool print(const char *string){return print(string,false);}
	static bool print(char ch) {return print(ch,false);}
	static bool print(int i){print(i,false);}
	static bool print(long i){print(i,false);}

	static bool println(const char *string){return print(string,true);}
	static bool println(char *string){return print(string,true);}
	static bool println(char ch){return print(ch,true);}
	static bool println(int i){print(i,true);}
	static bool println(long i){print(i,true);}


	static const char *getReceive();

	static byte getReceiveLength();

	static char *formatInt(char *digits, byte size, int value);
	static char *formatInt(char *digits, byte size, long value);

	static bool isFree(){return bus_free_;}

    static inline void ISR_TX();
    static inline void ISR_RX();
private:
	static char receive[MAX_TRANSMITT_LEN], transmitt_[MAX_TRANSMITT_LEN];
	static char * r;
	static void(*receive_finished_)(void);
	static void(*transmitt_finished_)(void);
	static byte receive_count_;
	static byte transmitt_count_;
	static byte transmitt_bytes_, received_bytes_;
	static bool str_direkt_;
	static bool bus_free_;
};
