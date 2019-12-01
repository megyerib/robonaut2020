#pragma once

#include "CyclicTask.h"
#include "TrackDetector.h"

class TrackTask : public CyclicTask
{
	TrackDetector* detector;

	TrackTask();

	void TaskInit() override;
	void TaskFunction() override;

public:
	static TrackTask* Init();
};
