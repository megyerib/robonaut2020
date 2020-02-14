#pragma once

#include "CyclicTask.h"
#include "Ui.h"

class TestTask : public CyclicTask
{
private:
	Ui* ui = nullptr;

	TestTask();

	void TaskInit() override;
	void TaskFunction() override;

public:
	static TestTask* Init();
};
