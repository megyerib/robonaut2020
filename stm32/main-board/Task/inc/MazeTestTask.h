#pragma once

#include "CyclicTask.h"

#include "Remote.h"
#include "Traction.h"
#include "Steering.h"
#include "TrackDetector.h"

class MazeTestTask : public CyclicTask
{
private:
	MazeTestTask();

	void TaskInit() override;
	void TaskFunction() override;

public:
	static MazeTestTask* Init();

private:
	Remote*        remote;
	Traction*      motor;
	Steering*      steering;
	TrackDetector* track;

	void RcRun();
	void Follow();
};
