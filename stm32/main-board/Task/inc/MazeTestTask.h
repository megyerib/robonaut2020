#pragma once

#include "CyclicTask.h"

#include "Remote.h"
#include "Traction.h"
#include "Steering.h"
#include "TrackDetector.h"
#include "Transmitter.h"
#include "Encoder.h"

class MazeTestTask : public CyclicTask
{
private:
	MazeTestTask();

	void TaskInit() override;
	void TaskFunction() override;

public:
	static MazeTestTask* Init();

private:
	Remote*        remote    = nullptr;
	Traction*      motor     = nullptr;
	Steering*      steering  = nullptr;
	TrackDetector* track     = nullptr;

	Transmitter*   trace     = nullptr;
	Encoder*       encoder   = nullptr;

	void RcRun();
	void Follow();
};
