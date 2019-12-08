#include "EncoderTask.h"
#include "TaskPrio.h"


EncoderTask::EncoderTask() : CyclicTask((char*)"Encoder", ENCODER_SAMPLING_PERIOD, ENCODER_TASK_PRIO, configMINIMAL_STACK_SIZE)
{

}

EncoderTask* EncoderTask::Init()
{
	static EncoderTask instance;
	return &instance;
}

void EncoderTask::TaskInit()
{
	encoder = Encoder::GetInstance();
}

void EncoderTask::TaskFunction()
{
	encoder->Process();
}
