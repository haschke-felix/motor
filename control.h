#include "avr.h"
#include "engine.h"
#include "portpin.h"
class Control
{
public:



	 Control();
	 void init();
	 void process();
	 void writeEnginePWM();
	 void initADC();
	 void accelerate();
	 byte getPedalSpeed();
	 void setPWM(byte pwm){pwm_ = pwm;}
	 byte getPWM(){return pwm_;}

private:
	 Engine  engine_;
	 int count_;
	 byte acceleration_counter_;
	 byte pwm_;
	 byte current_pwm_;
	 struct InputPin : PortPin{
		 bool current_state_;
//		 PortPin port_pin_;
	 };

	 InputPin charge1_;
	 InputPin charge2_;
	 InputPin discharge1_;
	 InputPin discharge2_;
};
