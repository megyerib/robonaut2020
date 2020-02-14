#include "MazeTestTask.h"
#include "TaskPrio.h"
#include "StringQueue.h"
#include "Trace.h"

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
	trace    = StringQueue::GetInstance(TestTrace);

	encoder = Encoder::GetInstance();

	track->SetMode(Speedrun);
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
	float speed;

	motor->SetMode(tmode_Controller);
	if (forward)
	{
	    speed = 1.2f;
		steering->SetMode(SingleLineFollow_Slow);

	}
	else if (reverse)
	{
		steering->SetMode(SteeringReverse);
		speed = -1.0f;
	}
	else
	{
	    motor->SetMode(tmode_Manual);
		motor->SetDutyCycle(0);
	}

	Trace::Print(trace, "v = %d mm/s", (int)(encoder->GetSpeed()*1000));

	motor->SetSpeed(speed);
}
