#include "WaitTime.h"

WaitTime::WaitTime()
{
	timer = Timepiece::GetInstance();
}

void WaitTime::Wait_s(int s)
{
	Wait_us(s * 1000000);
}

void WaitTime::Wait_ms(int ms)
{
	Wait_us(ms * 1000);
}

void WaitTime::Wait_us(int us)
{
	endTime = timer->GetTime() + us;
	started = true;
}

bool WaitTime::IsExpired()
{
	return (started && (endTime <= timer->GetTime()));
}
