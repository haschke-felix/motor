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

void Control::setCP1Mode(CapacitorMode mode){
	bool change = cp1_.mode ^ mode;
	cp1_.mode = mode;
	if(change){
		if(cp1_.mode == Charge){

		}

		else if(cp1_.mode == Discharge){

		}

		else if(cp1_.mode == Disabled){

		}
	}

}

void Control::setCP2Mode(CapacitorMode mode){
	cp2_.mode = mode;
}
