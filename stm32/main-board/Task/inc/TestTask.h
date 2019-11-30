#pragma once

#include "CyclicTask.h"
#include "ExtUart3.h"

class TestTask : public CyclicTask
{
	TestTask();

	ExtUart3* uart;

	void TaskInit() override;
	void TaskFunction() override;

public:
	static TestTask* Init();
};
