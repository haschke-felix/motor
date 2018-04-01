#include "avr.h"
class Control
{
public:
	 Control();
	 void init(volatile byte *send_buffer, volatile byte *receive_buffer, volatile bool *evaluate);
	 void process();
	 void evaluate();
private:
	 enum EngineMode : byte{
		  ON,
		  OFF,
	 };

	 enum CapacitorMode : byte{
		  Charge,
		  Discharge,
		  Nothing,
	 };

	 struct Capacitor{
		  CapacitorMode mode;
		  int time; // time of charge or discharge
		  int level;
	 };

	 struct Engine{
		  EngineMode mode;
		  byte PWM;
	 };

	 Engine engine_;
	 Capacitor cp1_;
	 Capacitor cp2_;

	 volatile byte * send_buffer_;
	 volatile byte * receive_buffer_;
	 volatile bool * evaluate_;
};
