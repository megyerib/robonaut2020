#include "TestTask.h"
#include "TaskPrio.h"

#include "Remote.h"
#include "StringQueue.h"

TestTask::TestTask() : CyclicTask((char*)"TEST", 2000, MAIN_TASK_PRIO, 256)
{

}

TestTask* TestTask::Init()
{
	static TestTask instance;
	return &instance;
}

void TestTask::TaskInit()
{
	ui = Ui::GetInstance();
}

void TestTask::TaskFunction()
{
	uint8_t state;
	bool    stopped;
	bool    cmd;

	stopped = ui->IsStopped();
	cmd = ui->GetCommand(&state);

	ui->SetCommand(40);

	UNUSED(state);
	UNUSED(stopped);
	UNUSED(cmd);
}
