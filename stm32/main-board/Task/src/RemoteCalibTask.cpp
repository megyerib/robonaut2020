#include "RemoteCalibTask.h"
#include "Remote.h"
#include "TaskPrio.h"
#include "Steering.h" /* For disabling servo */

RemoteCalibTask::RemoteCalibTask() : CyclicTask((char*)"Remote calibration", 50, MAIN_TASK_PRIO, configMINIMAL_STACK_SIZE)
{

}

RemoteCalibTask* RemoteCalibTask::Init()
{
	static RemoteCalibTask instance;
	return &instance;
}

void RemoteCalibTask::TaskInit()
{
	Steering::GetInstance(); // Disable servo
}

void RemoteCalibTask::TaskFunction()
{
	static Remote* remote = Remote::GetInstance();
	static bool calibrationStarted = false;

	if (!calibrationStarted)
	{
		if (remote->CalibrationStart(SteeringCh))
		{
			remote->CalibrationStart(ThrottleCh);
			calibrationStarted = true;
		}
	}
	else
	{
		remote->CalibrationProcess();

		if (!remote->IsCalibrationInProgress(SteeringCh) && !remote->IsCalibrationInProgress(ThrottleCh))
		{
			RemoteCal steeringCal, throttleCal;

			remote->GetCalibrationValues(SteeringCh, steeringCal);
			remote->GetCalibrationValues(ThrottleCh, throttleCal);

			// Set breakpoint here
			calibrationStarted = false; // Dummy command to provide place for breakpoint
			                            // Also it can restart calibration
		}
	}
}
