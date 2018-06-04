#pragma once
#include "avr.h"

struct PortPin{
	 volatile byte * port, * ddr, * in_pin;
	 byte pin;
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
