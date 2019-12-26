#include "System.h"
#include "LineDetectorLoop.h"
#include "SpiTestLoop.h"

int main(void)
{
	System::Init();

	LineDetectorLoop::Run();
	//SpiTestLoop::Run();
}

// TODO Időmérés (usec)
// TODO ütemező
// TODO NVIC prioritások
// TODO minden singleton
