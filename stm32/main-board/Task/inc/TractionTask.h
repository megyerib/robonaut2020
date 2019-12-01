#pragma once

#include "CyclicTask.h"
#include "Traction.h"

class TractionTask : public CyclicTask
{
	Traction* controller;

	TractionTask();

	void TaskInit() override;
	void TaskFunction() override;

public:
	static TractionTask* Init();
};
