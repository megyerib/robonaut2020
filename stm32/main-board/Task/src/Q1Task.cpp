#include "Q1Task.h"
#include "TaskPrio.h"

#include "Remote.h"

#define CYCLE_TIME 20 /* Remote reading only */

static Remote* remote;

Q1Task::Q1Task() : CyclicTask((char*)"Q1", CYCLE_TIME, MAIN_TASK_PRIO, configMINIMAL_STACK_SIZE)
{

}

Q1Task* Q1Task::Init()
{
	static Q1Task instance;
	return &instance;
}

void Q1Task::TaskInit()
{
	remote = Remote::GetInstance();
}

void Q1Task::TaskFunction()
{
	float throttle = remote->GetValue(ThrottleCh);
	float steering = remote->GetValue(SteeringCh);
	RemoteMode mode = remote->GetMode();

	(void) throttle;
	(void) steering;
	(void) mode;
}
