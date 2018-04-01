#include "datamanager.h"

DataManager::DataManager()
{

}

void DataManager::init(volatile byte * send_buffer, volatile byte * receive_buffer, volatile bool * evaluate){
	send_buffer_ = send_buffer;
	receive_buffer_ = receive_buffer;
	evaluate_ = evaluate;
	control_.init();
}

void DataManager::process(){
	if(*evaluate_){
		evaluate();
	}
}

void DataManager::evaluate(){

}