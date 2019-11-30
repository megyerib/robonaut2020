#pragma once

#include "Line.h"
#include "Uart.h"

class Comm
{
public:
	static Comm* GetInstance();
	void SendLine(Line* l);

private:
	Comm();
	Uart* uart;
};
