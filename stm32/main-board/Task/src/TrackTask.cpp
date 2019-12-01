#include "TrackTask.h"

TrackTask::TrackTask() : CyclicTask((char*)"Track", LINE_SAMPLING_CYCLE, 1, configMINIMAL_STACK_SIZE)
{

}

TrackTask* TrackTask::Init()
{
	static TrackTask instance;
	return &instance;
}

void TrackTask::TaskInit()
{
	detector = TrackDetector::GetInstance();
}

void TrackTask::TaskFunction()
{
	detector->Cycle();
}
