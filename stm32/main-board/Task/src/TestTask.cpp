#include "TestTask.h"
#include "RemoteHw.h"
#include "TaskPrio.h"

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

}

void TestTask::TaskFunction()
{
	static RemoteHw* remote = RemoteHw::GetInstance();

	uint16_t ch1 = remote->GetPulseWidth(RemCh1);
	uint16_t ch2 = remote->GetPulseWidth(RemCh2);

	(void) ch1;
	(void) ch2;
}
