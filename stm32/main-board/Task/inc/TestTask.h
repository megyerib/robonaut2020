#pragma once

#include <MotorUart.h>
#include "CyclicTask.h"

class TestTask : public CyclicTask
{
	TestTask();

	MotorUart* uart;

	void TaskInit() override;
	void TaskFunction() override;

public:
	static TestTask* Init();
};
