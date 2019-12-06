#pragma once

#include "CyclicTask.h"
#include "Steering.h"

class SteeringTask : public CyclicTask
{
	Steering* steering;

	SteeringTask();

	void TaskInit() override;
	void TaskFunction() override;

public:
	static SteeringTask* Init();
};
