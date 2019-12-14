#include "WaitDistance.h"

WaitDistance::WaitDistance()
{
	encoder = Encoder::GetInstance();
}

void WaitDistance::Wait_m(float m)
{
	startDistance = encoder->GetDistance();
	endDistance = startDistance + m;
	started = true;
}

bool WaitDistance::IsExpired()
{
	return (started && (endDistance <= encoder->GetDistance()));
}

float WaitDistance::GetDiff()
{
	return encoder->GetDistance() - startDistance;
}
