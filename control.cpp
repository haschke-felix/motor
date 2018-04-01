#include "control.h"

Control::Control()
{

}

void Control::init(volatile byte * send_buffer, volatile byte * receive_buffer, volatile bool * evaluate){
	send_buffer_ = send_buffer;
	receive_buffer_ = receive_buffer;
	evaluate_ = evaluate;
}

void Control::process(){
	if(evaluate_){
		evaluate();
	}
}

void Control::evaluate(){

}
