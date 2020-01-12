#include "TestTask.h"
#include "TaskPrio.h"

#include "Starter.h"

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
	static Starter* starter = Starter::GetInstance();

	StarterState state = starter->GetState();

	(void) state;
}
