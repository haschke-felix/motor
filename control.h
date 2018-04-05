#include "avr.h"
class Control
{
public:
	enum EngineMode : byte{
		 ON,
		 OFF,
	};

	enum CapacitorMode : byte{
		Charge,
		Discharge,
		Disabled,
	};
	 Control();
	 void init();
	 void process();
	 void setPWM(byte pwm);
	 byte getPWM() {return engine_.PWM;}
	 void setCP1Mode(CapacitorMode mode);
	 CapacitorMode getCP1Mode(){return cp1_.mode;}
	 void setCP2Mode(CapacitorMode mode);
	 CapacitorMode getCP2Mode(){return cp2_.mode;}
	 void writeEnginePWM();
private:

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
