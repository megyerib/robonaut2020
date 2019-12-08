#pragma once

#include "CyclicTask.h"

class Q1Task : public CyclicTask
{
	Q1Task();

	void TaskInit() override;
	void TaskFunction() override;

public:
	static Q1Task* Init();
};
