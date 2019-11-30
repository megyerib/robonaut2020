#pragma once

#include "Line.h"

class Comm
{
public:
	static Comm* GetInstance();
	void SendLine(Line l);

private:
	Comm();
};
