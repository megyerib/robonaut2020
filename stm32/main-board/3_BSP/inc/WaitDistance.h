#pragma once

#include "Encoder.h"

class WaitDistance
{
public:
	WaitDistance();
	void Wait_m(float m);
	bool IsExpired();
private:
	float endDistance;
	bool started = false;
	Encoder* encoder;
};
