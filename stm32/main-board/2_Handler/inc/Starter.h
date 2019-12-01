#pragma once

#include "StarterUart.h"

typedef enum
{
	Go,
	Countdown1,
	Countdown2,
	Countdown3,
	Countdown4,
	Countdown5,
	NoSignal
}
StarterState;

class Starter
{
public:
	static Starter* GetInstance();
	StarterState GetState();

private:
	StarterUart* uart;

	Starter();
};
