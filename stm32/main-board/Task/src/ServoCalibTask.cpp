#include "ServoCalibTask.h"
#include "Remote.h"
#include "TaskPrio.h"
#include "Steering.h"

#define SPEED_FACTOR      0.03f

ServoCalibTask::ServoCalibTask() : CyclicTask((char*)"Servo calibration", 50, MAIN_TASK_PRIO, configMINIMAL_STACK_SIZE)
{

}

ServoCalibTask* ServoCalibTask::Init()
{
	static ServoCalibTask instance;
	return &instance;
}

void ServoCalibTask::TaskInit()
{
	Steering* s = Steering::GetInstance();
	s->SetMode(Free);
}

void ServoCalibTask::TaskFunction()
{
	static Steering* steering = Steering::GetInstance();
	static Remote*   remote   = Remote::GetInstance();

	static float frontAngle = 0;
	static float rearAngle  = 0;

	if (remote->GetMode() == RemMode1)
	{
		// Front
		frontAngle -= remote->GetValue(SteeringCh) * SPEED_FACTOR;
	}
	else
	{
		// Rear
		rearAngle -= remote->GetValue(SteeringCh) * SPEED_FACTOR;
	}

	steering->SetAngleManual(frontAngle, rearAngle);
}
