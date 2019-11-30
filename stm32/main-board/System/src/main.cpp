#include "System.h"
#include "OSTasks.h"

int main(void)
{
	System* system = System::GetInstance();

	// Module & task initializations
	TestTask::Init();

	system->OsStart();

	while (1);
}
