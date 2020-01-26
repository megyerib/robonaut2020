#include "TestTask.h"
#include "TaskPrio.h"

#include "Remote.h"
#include "StringQueue.h"

TestTask::TestTask() : CyclicTask((char*)"TEST", 1000, MAIN_TASK_PRIO, 256)
{
	msgQueue = StringQueue::GetInstance(TestTrace);
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
	static char buffer1[] = "Test";
	static char buffer2[] = "ASD";
	msgQueue->Transmit(buffer1, 4);
	msgQueue->Transmit(buffer2, 3);
}
