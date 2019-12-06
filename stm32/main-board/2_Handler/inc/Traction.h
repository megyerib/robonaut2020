#pragma once

#include "MotorUart.h"

#define TRACTION_CONTROL_CYCLE   10 /* ms */

typedef enum
{
	tmodeAccel,
	tmodeBraking
	// ...
}
TractionMode;

class Traction
{
public:
	static Traction* GetInstance();
	void SetSpeed(float speed /* m/s */);
	void SetMode(TractionMode mode);
	void Process();

private:
	MotorUart* uart;
	float targetSpeed;
	TractionMode mode;
	float targetDutyCycle; // TODO remove
	void SetDutyCycle(float d /* % [-100;+100] */);

	Traction();
};
