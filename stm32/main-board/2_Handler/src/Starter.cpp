#include "Starter.h"

Starter::Starter()
{
	uart = StarterUart::GetInstance();
}

Starter* Starter::GetInstance()
{
	static Starter instance;
	return &instance;
}

StarterState Starter::GetState()
{
	StarterState ret = NoSignal;
	uint8_t c = uart->GetLatestChar();

	switch (c)
	{
		case '5': ret = Countdown5; break;
		case '4': ret = Countdown4; break;
		case '3': ret = Countdown3; break;
		case '2': ret = Countdown2; break;
		case '1': ret = Countdown1; break;
		case '0': ret = Go;         break;
		default:  ret = NoSignal;   break;
	}

	return ret;
}
