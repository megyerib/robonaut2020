#include "TestTask.h"
#include "TaskPrio.h"

#include "Traction.h"

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
	static Traction* traction = Traction::GetInstance();

	traction->SetDutyCycle(0.2);
	traction->Process();
}
