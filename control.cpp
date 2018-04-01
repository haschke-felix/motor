#include "control.h"

Control::Control()
{

}

void Control::init(){

}

void Control::process(){

}

void Control::setPWM(byte pwm){
	engine_.PWM = pwm;
	writeEnginePWM();
}

void Control::writeEnginePWM(){

}
