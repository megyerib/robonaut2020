#include "TraceTask.h"
#include "TaskPrio.h"

#define TRACE_CYCLE   (1000u)

TraceTask::TraceTask() : CyclicTask((char*)"Trace", TRACE_CYCLE, TRACE_TASK_PRIO, configMINIMAL_STACK_SIZE) // TODO increase stack size
{

}

TraceTask* TraceTask::Init()
{
	static TraceTask instance;
	return &instance;
}

void TraceTask::TaskInit()
{
	trace = Trace::GetInstance();
}

void TraceTask::TaskFunction()
{
	trace->Process();
}
