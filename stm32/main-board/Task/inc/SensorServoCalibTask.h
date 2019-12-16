#pragma once

#include "CyclicTask.h"

class SensorServoCalibTask : public CyclicTask
{
private:
	SensorServoCalibTask();

	void TaskInit() override;
	void TaskFunction() override;

public:
	static SensorServoCalibTask* Init();
};
