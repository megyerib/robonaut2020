#pragma once

#include "cyclictask.h"

class LedTask : public CyclicTask
{
protected:
	void TaskInit() override;
	void TaskFunction() override;

private:
	LedTask();

public:
	static LedTask* Create();
};
