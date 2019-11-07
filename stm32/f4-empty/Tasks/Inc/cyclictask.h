/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  TASK.H
//  Base class for the task classes.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "cyclictask.h"

#include "FreeRTOS.h"
#include "task.h"

class CyclicTask
{
private:
	TaskHandle_t handle;
	uint16_t period;

	void TaskLoop();
	static void TaskFunctionAdapter(void* p);

protected:
	CyclicTask();
	CyclicTask(char* name, uint16_t period, uint16_t prio, uint16_t stack_size);
	~CyclicTask();
	void InitTask(char* name, uint16_t period, uint16_t prio, uint16_t stack_size);


	virtual void TaskInit() = 0;
	virtual void TaskFunction() = 0;
};
