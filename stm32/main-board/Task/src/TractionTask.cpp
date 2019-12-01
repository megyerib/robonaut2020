#include "TractionTask.h"

TractionTask::TractionTask() : CyclicTask((char*)"Traction", TRACTION_CONTROL_CYCLE, 1, configMINIMAL_STACK_SIZE)
{

}

TractionTask* TractionTask::Init()
{
	static TractionTask instance;
	return &instance;
}

void TractionTask::TaskInit()
{
	controller = Traction::GetInstance();
}

void TractionTask::TaskFunction()
{
	controller->Process();
}
