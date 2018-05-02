#include "avr.h"
#include "engine.h"
#include "capacitor.h"
class Control
{
public:



	 Control();
	 void init();
	 void process();
	 void writeEnginePWM();

	 Engine engine_;
	 Capacitor cp1_;
	 Capacitor cp2_;
private:
};
