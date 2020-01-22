#include "TestTask.h"
#include "TaskPrio.h"

#include "Remote.h"

TestTask::TestTask() : CyclicTask((char*)"TEST", 1000, MAIN_TASK_PRIO, 256)
{

}

TestTask* TestTask::Init()
{
	static TestTask instance;
	return &instance;
}

void TestTask::TaskInit()
{

}

void TestTask::TaskFunction()
{
	static Remote* remote = Remote::GetInstance();

	float ch1 = remote->GetValue(SteeringCh);
	float ch2 = remote->GetValue(ThrottleCh);
	RemoteMode ch3 = remote->GetMode();

	UNUSED(ch1);
	UNUSED(ch2);
	UNUSED(ch3);
}
