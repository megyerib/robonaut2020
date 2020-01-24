#include "TestTask.h"
#include "TaskPrio.h"
#include "Steering.h"
#include "Traction.h"
#include "TrackDetector.h"

#include "RemoteHw.h"

#include "Distance.h"

static Steering* steering;
static Traction* motor;
static TrackDetector* track;
static Distance*    distance;
static float srv_angle;

TestTask::TestTask() : CyclicTask((char*)"TEST", 20, MAIN_TASK_PRIO, 256)
{

}

TestTask* TestTask::Init()
{
	static TestTask instance;

	return &instance;
}

void TestTask::TaskInit()
{
	steering = Steering::GetInstance();
	steering->EnableSteering(true);
	steering->SetMode(Free);
	steering->SetAngleManual(0, 0);
	steering->SetMode(SingleLineFollow_Slow);

	motor = Traction::GetInstance();
	motor->SetSpeed(0.0);

	track = TrackDetector::GetInstance();

	distance = Distance::GetInstance();
	srv_angle = 0.0f;
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


	track->Process();

	steering->SetLine(track->GetFrontLine(), 0);
	distance->SetFrontServo(steering->GetFrontAngle());

	distance->Process();

//    float dst = distance->GetSharpDistance();
//
//    if (dst < 400 && dst > 0)
//    {
//        // Speed.
//        if (dst > 60)
//        {
//            dst = 0;
//        }
//        else if (dst < 40)
//        {
//            dst = 4;
//        }
//        else
//        {
//            dst = 22;
//        }
//    }
}
