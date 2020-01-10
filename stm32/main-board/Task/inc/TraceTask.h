#pragma once

#include "CyclicTask.h"
#include "Trace.h"

class TraceTask : public CyclicTask
{
	Trace* trace;

	TraceTask();

	void TaskInit() override;
	void TaskFunction() override;

public:
	static TraceTask* Init();
};
