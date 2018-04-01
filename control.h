#include "avr.h"
class Control
{
public:
	 Control();
	 void init();
	 void process();
	 void setPWM(byte pwm);
	 byte getPWM() {return engine_.PWM;}
	 void writeEnginePWM();
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
};
