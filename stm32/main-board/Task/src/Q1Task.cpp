#include "Q1Task.h"
#include "TaskPrio.h"

#define CYCLE_TIME 20 /* Remote reading only */

#define RC_THROTTLE_THRESHOLD    (0.1f)
#define RC_THROTTLE_FUN_FACTOR   (0.4f)

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
    car      = Car::GetInstance();
	remote   = Remote::GetInstance();
	motor    = Traction::GetInstance();

	steering = Steering::GetInstance();
	steering->SetMode(Free);
	steering->EnableSteering(true);
}

void Q1Task::TaskFunction()
{
	float      throttle   = remote->GetValue(ThrottleCh);
	float      steerAngle = remote->GetValue(SteeringCh);
	RemoteMode mode       = remote->GetMode();

	if (mode == RemMode1)
	{
		car->StateMachine();
	}

	if (mode == RemMode2)
	{
		RcRun(throttle, steerAngle);
	}
}

void Q1Task::RcRun(float throttle, float steerAngle)
{
	if (throttle > RC_THROTTLE_THRESHOLD || throttle < -RC_THROTTLE_THRESHOLD)
	{
		motor->SetDutyCycle(throttle * RC_THROTTLE_FUN_FACTOR); // Don't use the full scale
	}
	else
	{
		motor->SetDutyCycle(0);
	}

	steering->SetAngleManual(-steerAngle, steerAngle);
}
