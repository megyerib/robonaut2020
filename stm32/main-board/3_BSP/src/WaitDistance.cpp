#include "WaitDistance.h"

WaitDistance::WaitDistance()
{
	encoder = Encoder::GetInstance();
}

void WaitDistance::Wait_m(float m)
{
	endDistance = encoder->GetDistance() + m;
	started = true;
}

bool WaitDistance::IsExpired()
{
	return (started && (endDistance <= encoder->GetDistance()));
}
