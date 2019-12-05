#pragma once

#include "CyclicTask.h"

class TestTask : public CyclicTask
{
	TestTask();

	void TaskInit() override;
	void TaskFunction() override;

public:
	static TestTask* Init();
};
