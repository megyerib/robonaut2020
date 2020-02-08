#include "MazeTestTask.h"
#include "TaskPrio.h"

#define CYCLE_TIME 10

#define RC_THROTTLE_THRESHOLD    (0.1f)
#define RC_THROTTLE_FUN_FACTOR   (0.25f)

MazeTestTask::MazeTestTask() : CyclicTask((char*)"MazeTest", CYCLE_TIME, MAIN_TASK_PRIO, 256)
{

}

MazeTestTask* MazeTestTask::Init()
{
	static MazeTestTask instance;
	return &instance;
}

void MazeTestTask::TaskInit()
{
	remote   = Remote::GetInstance();
	motor    = Traction::GetInstance();
	steering = Steering::GetInstance();
	track    = TrackDetector::GetInstance();
}

void MazeTestTask::TaskFunction()
{
	RemoteMode mode = remote->GetMode();

	if (mode == RemMode1)
	{
		float frontLine = track->GetFrontLine();
		float rearLine  = track->GetRearLine();
		steering->SetLine(frontLine, rearLine);

		Follow();
	}

	if (mode == RemMode2)
	{
		RcRun();
	}
}

void MazeTestTask::RcRun()
{
	steering->SetMode(Free);

	float throttle   = remote->GetValue(ThrottleCh);
	float steerAngle = remote->GetValue(SteeringCh);

	if (throttle > RC_THROTTLE_THRESHOLD || throttle < -RC_THROTTLE_THRESHOLD)
	{
		motor->SetDutyCycle(throttle * RC_THROTTLE_FUN_FACTOR); // Don't use the full scale
	}
	else
	{
		motor->SetDutyCycle(0);
	}

	steering->SetAngleManual(-steerAngle, steerAngle);
}

void MazeTestTask::Follow()
{
	float throttle  = remote->GetValue(RemoteChannel::ThrottleCh);
	bool  forward   = throttle >= RC_THROTTLE_THRESHOLD;
	bool  reverse   = throttle <= -RC_THROTTLE_THRESHOLD;

	if (forward)
	{
		steering->SetMode(SingleLineFollow_Slow);
		motor->SetDutyCycle(0.15);
	}
	else if (reverse)
	{
		steering->SetMode(SteeringReverse);
		motor->SetDutyCycle(-0.12);
	}
	else
	{
		motor->SetDutyCycle(0);
	}
}
