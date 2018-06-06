#pragma once
#include "avr.h"

struct PortPin{
	volatile byte * r_port, * r_ddr, * r_pin;
	byte pin;
	void clear(){
		bitClear(*r_port,pin);
	}
	void set(){
		bitSet(*r_port,pin);
	}
	void write(bool value){
		bitWrite(*r_port,pin,value);
	}
	bool read(){
		return bitRead(*r_pin,pin);
	}

	void output(){
		bitSet(*r_ddr,pin);
	}
	void input(){
		bitClear(*r_ddr,pin);
	}

	void setPins(volatile byte *r_port, volatile byte *r_ddr, volatile byte * r_pin, byte pin){
		PortPin::r_pin = r_pin;
		PortPin::r_port = r_port;
		PortPin::r_ddr = r_ddr;
		PortPin::pin = pin;
	}
};

namespace PortPins {
inline PortPin set(volatile byte *port, volatile byte *ddr, byte pin){
	PortPin portpin;
	portpin.r_port = port;
	portpin.r_ddr = ddr;
	portpin.pin = pin;
	return portpin;
}
inline PortPin set(volatile byte *r_port, volatile byte *r_ddr, volatile byte * r_pin, byte pin){
	PortPin portpin;
	portpin.r_pin = r_pin;
	portpin.r_port = r_port;
	portpin.r_ddr = r_ddr;
	portpin.pin = pin;
	return portpin;
}
}
