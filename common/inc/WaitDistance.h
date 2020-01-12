#pragma once

#include "Encoder.h"

class WaitDistance
{
public:
	WaitDistance();
	void Wait_m(float m);
	bool IsExpired();
	float GetDiff();
private:
	float startDistance;
	float endDistance;
	bool started = false;
	Encoder* encoder;
};
