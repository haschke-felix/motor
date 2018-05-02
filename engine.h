#pragma once
#include "avr.h"

class Engine
{
public:
	 Engine();

	 enum EngineMode : byte{
		  ON,
		  OFF,
	 };

	 EngineMode mode;
	 byte PWM;

};
