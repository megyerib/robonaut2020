#pragma once

#include "Servo.h"

typedef enum
{
	DualLineFollow,
	SingleLineFollow,
	Free,             // Angles are set manually
	Reverse
	// ...
} SteeringMode;

class Steering
{
public:
	static Steering* GetInstance();
	void SetMode(SteeringMode mode);
	void SetLine(float front_line /* m */, float rear_line /* m */); // For LineFollow modes
	void SetAngle(float front_angle /* rad */, float rear_angle /* rad */); // For Free mode
	void Process();

private:
	SteeringMode mode;
	Servo*       srv_front;
	Servo*       srv_rear;

	float front_angle;
	float rear_angle;

	Steering();
};
