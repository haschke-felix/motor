#pragma once
#include "avr.h"

struct PortPin{
	 volatile byte * port, * ddr;
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
}
