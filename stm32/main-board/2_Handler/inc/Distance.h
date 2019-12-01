#pragma once

#define DISTANCE_SAMPLING_CYCLE   100 /* ms */

typedef enum
{
	ToF1,
	ToF2,
	ToF3
}
DistanceSensor;

class Distance
{
public:
	static Distance* GetInstance();
	float GetDistance(DistanceSensor sensor); /* m */
	float SetFrontServo(float angle /* rad */);
	void Process();

private:
	Distance();
};
