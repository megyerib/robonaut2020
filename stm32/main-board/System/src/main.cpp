#include "System.h"
#include "OSTasks.h"

int main(void)
{
	// System init (must be done first!)
	System* system = System::GetInstance();

	// Module & task initializations

		// Service tasks
		TrackTask    :: Init();
		TractionTask :: Init();
		SteeringTask :: Init();
		EncoderTask  :: Init();
		DistanceTask :: Init();

		// Application tasks
		Q1Task       :: Init();
	    //TestTask     :: Init();
		//ServoCalibTask :: Init();


	system->OsStart();

	while (1);
}
