#pragma once
#include "avr.h"

struct PortPin{
	volatile byte * port, * ddr, * in_pin;
	byte pin;
	void clear(){
		bitClear(*port,pin);
	}
	void set(){
		bitSet(*port,pin);
	}
	void write(bool value){
		bitWrite(*port,pin,value);
	}
	bool read(){
		return bitRead(*in_pin,pin);
	}

	void output(){
		bitSet(*ddr,pin);
	}
	void input(){
		bitClear(*ddr,pin);
	}

	void setPins(volatile byte *port, volatile byte *ddr, volatile byte * in_pin, byte pin){
		PortPin::in_pin = in_pin;
		PortPin::port = port;
		PortPin::ddr = ddr;
		PortPin::pin = pin;
	}
};

namespace PortPins {
inline PortPin set(volatile byte *port, volatile byte *ddr, byte pin){
	PortPin portpin;
	portpin.port = port;
	portpin.ddr = ddr;
	portpin.pin = pin;
	return portpin;
}
inline PortPin set(volatile byte *port, volatile byte *ddr, volatile byte * in_pin, byte pin){
	PortPin portpin;
	portpin.in_pin = in_pin;
	portpin.port = port;
	portpin.ddr = ddr;
	portpin.pin = pin;
	return portpin;
}
}
