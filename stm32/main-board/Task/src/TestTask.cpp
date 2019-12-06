#include "TestTask.h"
#include "TaskPrio.h"
#include "Steering.h"
#include "Traction.h"
#include "TrackDetector.h"

#include "RemoteHw.h"

static Steering* steening;
static Traction* motor;
static TrackDetector* track;

TestTask::TestTask() : CyclicTask((char*)"TEST", 500, TEST_TASK_PRIO, 256)
{

}

TestTask* TestTask::Init()
{
	static TestTask instance;

	return &instance;
}

void TestTask::TaskInit()
{
	steening = Steering::GetInstance();
	steening->EnableSteering(true);
	steening->SetMode(Free);
	steening->SetAngleManual(0, 0);

	motor = Traction::GetInstance();
	motor->SetSpeed(0.0);

	track = TrackDetector::GetInstance();
}

void TestTask::TaskFunction()
{
	static RemoteHw* remote = RemoteHw::GetInstance();

	uint16_t ch1 = remote->GetPulseWidth(RemCh1);
	uint16_t ch2 = remote->GetPulseWidth(RemCh2);
	uint16_t ch3 = remote->GetPulseWidth(RemCh3);

	(void) ch1;
	(void) ch2;
	(void) ch3;
}
