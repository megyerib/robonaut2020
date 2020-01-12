#include <RemoteHw.h>
#include "TestTask.h"
#include "TaskPrio.h"


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
