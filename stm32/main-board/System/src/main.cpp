#include "System.h"
#include "OSTasks.h"

int main(void)
{
	// System init (must be done first!)
	System* system = System::GetInstance();

	// Module & task initializations
	TrackTask    :: Init();
	TractionTask :: Init();
	SteeringTask :: Init();

	Q1Task       :: Init();

	system->OsStart();

	while (1);
}
