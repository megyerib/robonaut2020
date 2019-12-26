#include "SpiTestLoop.h"

#include "Display.h"
#include "SensorDriver.h"

void SpiTestLoop::Run()
{
	SensorDriver sensors;
	Display      display;

	while(1)
	{
		static uint32_t buf = 1;

		sensors.DisplayPattern(buf);
		display.DisplayPattern(buf);

		// BREAKPOINT

		buf <<= 1;

		if (buf == 0)
		{
			buf = 1;
		}
	}
}
