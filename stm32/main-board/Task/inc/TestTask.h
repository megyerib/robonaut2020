#pragma once

#include "RemoteHw.h"
#include "CyclicTask.h"

class TestTask : public CyclicTask
{
	TestTask();

	void TaskInit() override;
	void TaskFunction() override;

	RemoteHw* remote;

public:
	static TestTask* Init();
};
