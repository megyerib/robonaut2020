#pragma once

#include "CyclicTask.h"
#include "Transmitter.h"

class TestTask : public CyclicTask
{
private:
	Transmitter* msgQueue = nullptr;

	TestTask();

	void TaskInit() override;
	void TaskFunction() override;

public:
	static TestTask* Init();
};
