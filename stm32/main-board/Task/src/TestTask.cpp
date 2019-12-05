#include "TestTask.h"
#include "EncoderHw.h"

TestTask::TestTask() : CyclicTask((char*)"TEST", 500, 1, configMINIMAL_STACK_SIZE)
{
	remote = RemoteHw::GetInstance();
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
	uint16_t ch1 = remote->GetPulseWidth(RemCh1);
	uint16_t ch2 = remote->GetPulseWidth(RemCh2);
}
