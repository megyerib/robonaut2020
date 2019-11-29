#include "Display.h"
#include "SensorCfg.h"

Display::Display() : leds(10, 2)
{
	uint32_t buf = 0;
	leds.Display(&buf, 4); // TODO maybe make a 'clear' function
}

void Display::DisplayLinePos(Line l)
{
	uint32_t ledval = 0, i, ledpos = 0;

	for (i = 0; i < l.cnt; i++)
	{
		ledpos = mmToLedPos(l.lines[i]);

		ledval |= 0b11 << ledpos;
	}

	leds.Display(&ledval, 4);
}

uint8_t Display::mmToLedPos(int16_t mm) // TODO typedef for this?
{
    uint8_t ledPos = (MID_IR_POS_MM - mm) / IR_DIST_MM;

    return ledPos;
}
