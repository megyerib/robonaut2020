#pragma once

#include "CyclicTask.h"

#include "Car.h"
#include "Remote.h"
#include "Traction.h"
#include "Steering.h"

class Q1Task : public CyclicTask
{
public:
	static Q1Task* Init();

private:
	Q1Task();

	Car*      car;
	Remote*   remote;
	Traction* motor;
	Steering* steering;

	void TaskInit() override;
	void TaskFunction() override;

	void RcRun(float throttle, float steerAngle);
};
