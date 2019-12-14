#pragma once

#include "CyclicTask.h"

class ServoCalibTask : public CyclicTask
{
private:
	ServoCalibTask();

	void TaskInit() override;
	void TaskFunction() override;

public:
	static ServoCalibTask* Init();
};
