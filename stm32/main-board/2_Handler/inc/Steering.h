#pragma once

#include "Servo.h"

#define STEERING_CYCLE_TIME    10 /* ms */

typedef enum
{
	Off,
	//DualLineFollow,
	SingleLineFollow,
	Free,             // Angles are set manually
	//Reverse
	// ...
} SteeringMode;

class Steering
{
public:
	static Steering* GetInstance();
	void SetMode(SteeringMode mode);
	void SetLine(float front_line /* m */, float rear_line /* m */); // For LineFollow modes
	void SetAngleManual(float front_angle /* rad */, float rear_angle /* rad */); // For Free mode
	void Process();
	void EnableSteering(bool enable);

private:
	SteeringMode mode;
	Servo*       srv_front;
	Servo*       srv_rear;

	float front_angle;
	float rear_angle;

	Steering();

	void SetFrontAngle(float angle /* rad */);
	void SetRearAngle(float angle /* rad */);

	void InitEnablePin();
};
