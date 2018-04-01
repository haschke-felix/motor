#include "avr.h"

enum SEND_DATA : byte{
	Speed_front_right		= 0,		// Speed of right font whell
	Speed_front_left		= 2,		// Speed of left font whell
	Speed_rear_right		= 4,		// Speed of right rear whell
	Speed_rear_left		= 6,		// Speed of left rear whell

	BatteryVoltage			= 8,		// Battery Voltage

	BatteryCurrent			= 12,		// Batery Current

	Motor_PWM				= 16,		// PWM for the motor

	Brake_1					= 17,		// Brake

	CP_Mode					= 37,		// Cp Mode
	CP1_Voltage				= 19,		// Capacitor Pack 1 Voltage
	CP2_Voltage				= 23,		// Capacitor Pack 1 Voltage

	Engine_RPM				= 27,		// Speed of the engine
	Engine_Voltage			= 29,		// Volatage of the Main Engine
	Engine_Current			= 33,		// Current of the Main Engine

	End_SEND_DATA			= 38,		// last item
};
