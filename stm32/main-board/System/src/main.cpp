#include "System.h"
#include "OSTasks.h"

int main(void)
{
	// System init (must be done first!)

	System* system = System::GetInstance();

	// Service tasks

	TrackTask      :: Init();
	TractionTask   :: Init();
	SteeringTask   :: Init();
	EncoderTask    :: Init();
	DistanceTask   :: Init();
	TraceTask      :: Init();
	//NavigationTask :: Init();

	// Application tasks

	//Q1Task         :: Init();
	//TestTask       :: Init();
	//ServoCalibTask :: Init();
	//SensorServoCalibTask :: Init();
	MazeTestTask   :: Init();

	system->OsStart();

	while (1);
}
