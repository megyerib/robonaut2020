#pragma once

#include "MotorUart.h"
#include "Encoder.h"

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
	void SetDutyCycle(float d /* % [-1;+1] */);
	void SetMode(TractionMode mode);

	void Process();

private:
	MotorUart* uart;
	Encoder* encoder;

	//float targetSpeed;
	float targetDutyCycle;
	TractionMode mode;

	Traction();

	void SendDutyCycle(float d /* % [-1;+1] */);
};
