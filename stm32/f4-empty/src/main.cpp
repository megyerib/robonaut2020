#include "system.h"
#include "ledtask.h"


int main(void)
{
	System* system = System::Get();

	LedTask::Create();

	system->OsStart();

	while (1);
}

