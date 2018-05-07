#include "avr.h"
#include "control.h"
#pragma once
class DataManager
{
public:
	DataManager();
	void init(volatile byte *send_buffer, volatile byte *receive_buffer, volatile bool *evaluate);
////	void process();
////	void evaluate();

//private:
////	volatile byte * send_buffer_;
////	volatile byte * receive_buffer_;
////	volatile bool * evaluate_;
	Control control_;
};

class foo2{
public:
	foo2();
	void init();
};


