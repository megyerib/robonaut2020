#include "ServoCalibTask.h"
#include "Remote.h"
#include "TaskPrio.h"
#include "Steering.h"
#include "StringQueue.h"
#include "Trace.h"
#include "Traction.h"

#define SPEED_FACTOR      0.03f

ServoCalibTask::ServoCalibTask() : CyclicTask((char*)"Servo calibration", 25, MAIN_TASK_PRIO, 256)
{

}

ServoCalibTask* ServoCalibTask::Init()
{
	static ServoCalibTask instance;
	return &instance;
}

void ServoCalibTask::TaskInit()
{
	trace = StringQueue::GetInstance(ServoCalib);

	Steering* s = Steering::GetInstance();
	s->SetMode(Free);
}

void ServoCalibTask::TaskFunction()
{
	static Steering* steering = Steering::GetInstance();
	static Remote*   remote   = Remote::GetInstance();
	static Traction* motor    = Traction::GetInstance();

	static float frontAngle = 0;
	static float rearAngle  = 0;

	float throttle = remote->GetValue(ThrottleCh);
	float angle    = remote->GetValue(SteeringCh);

	if (throttle < 0.05 && throttle > -0.05)
		throttle = 0;

	if (angle < 0.05 && angle > -0.05) // To avoid too much integration error
		angle = 0;

	throttle *= 0.25;
	angle    *= SPEED_FACTOR;

	if (remote->GetMode() == RemMode1)
	{
		// Front
		frontAngle -= angle;
	}
	else
	{
		// Rear
		rearAngle -= angle;
	}

	steering->SetAngleManual(frontAngle, rearAngle);

	//Trace::Print(trace, "%d;%d", (int)(frontAngle*1000), (int)(rearAngle*1000));



	motor->SetDutyCycle(throttle);
}
