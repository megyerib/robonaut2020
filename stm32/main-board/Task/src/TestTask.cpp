#include "TestTask.h"
#include "EncoderHw.h"

TestTask::TestTask() : CyclicTask((char*)"TEST", 500, 1, configMINIMAL_STACK_SIZE)
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
	EncoderHw* enc = EncoderHw::GetInstance();

	uint32_t encval = enc->GetCounterValue();
}
