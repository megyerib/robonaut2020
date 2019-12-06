#include "System.h"
#include "OSTasks.h"

int main(void)
{
	// System init (must be done first!)
	System* system = System::GetInstance();

	// Module & task initializations
	RemoteCalibTask::Init();

	system->OsStart();

	while (1);
}
