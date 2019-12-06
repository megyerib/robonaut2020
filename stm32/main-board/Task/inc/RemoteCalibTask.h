#pragma once

#include "CyclicTask.h"

class RemoteCalibTask : public CyclicTask
{
private:
	RemoteCalibTask();

	void TaskInit() override;
	void TaskFunction() override;

public:
	static RemoteCalibTask* Init();
};
