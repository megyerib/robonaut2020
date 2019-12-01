#include "TestTask.h"
#include "Line.h"
#include "StarterUart.h"
#include "base64.h"

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
	uart = MotorUart::GetInstance();
}

void TestTask::TaskFunction()
{
	uart->Send((uint8_t*)"42069\r\n", 7);
}
