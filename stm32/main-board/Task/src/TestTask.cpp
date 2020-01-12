#include "TestTask.h"
#include "TaskPrio.h"

#include "RemoteHwDma.h"

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
	static RemoteHwDma* remote = RemoteHwDma::GetInstance();

	uint16_t ch1 = remote->GetPulseWidth(RemCh1);

	(void) ch1;
}
