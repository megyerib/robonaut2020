#include "SensorServoCalibTask.h"
#include "Remote.h"
#include "TaskPrio.h"
#include "Distance.h"

#define SPEED_FACTOR      0.03f

SensorServoCalibTask::SensorServoCalibTask() : CyclicTask((char*)"Sensor servo calibration", 50, MAIN_TASK_PRIO, 512)
{

}

SensorServoCalibTask* SensorServoCalibTask::Init()
{
	static SensorServoCalibTask instance;
	return &instance;
}

void SensorServoCalibTask::TaskInit()
{

}

void SensorServoCalibTask::TaskFunction()
{
	static Remote* remote = Remote::GetInstance();
	static Distance* dst = Distance::GetInstance();

	static float angle = 0;

	angle -= remote->GetValue(SteeringCh) * SPEED_FACTOR;

	dst->SetFrontServo(angle);

	// Set angle

}
