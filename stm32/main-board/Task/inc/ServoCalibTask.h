#pragma once

#include "CyclicTask.h"
#include "Transmitter.h"

class ServoCalibTask : public CyclicTask
{
public:
	static ServoCalibTask* Init();

private:
	ServoCalibTask();

	void TaskInit() override;
	void TaskFunction() override;

	Transmitter* trace;
};
