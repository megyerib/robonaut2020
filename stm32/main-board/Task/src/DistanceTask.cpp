#include "DistanceTask.h"
#include "TaskPrio.h"

DistanceTask::DistanceTask() : CyclicTask((char*)"Distance", DISTANCE_SAMPLING_CYCLE, DISTANCE_TASK_PRIO, configMINIMAL_STACK_SIZE)
{

}

DistanceTask* DistanceTask::Init()
{
    static DistanceTask instance;
    return &instance;
}

void DistanceTask::TaskInit()
{
    distance = Distance::GetInstance();
}

void DistanceTask::TaskFunction()
{
    distance->Process();
}
