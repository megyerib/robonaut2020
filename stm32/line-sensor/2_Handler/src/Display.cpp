#include "Display.h"
#include "SensorCfg.h"

Display::Display() : leds(10, 2)
{
	Clear();
}

void Display::DisplayLinePos(Line l)
{
	uint32_t ledval = 0, i, ledpos = 0;

	for (i = 0; i < l.cnt; i++)
	{
		ledpos = mmToLedPos(l.lines[i]);

		ledval |= 0b11 << ledpos;
	}

	leds.Display(&ledval, GROUP_CNT);
}

uint8_t Display::mmToLedPos(int16_t mm)
{
    uint8_t ledPos = (MID_IR_POS_MM - mm) / IR_DIST_MM;

    return ledPos;
}

void Display::Clear()
{
	uint32_t buf = 0;

	leds.Display((uint8_t*)&buf, GROUP_CNT);
}
