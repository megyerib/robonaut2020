#pragma once

#include "Line.h"
#include "LedDriver.h"

class Display
{
	LedDriver leds;

public:
	Display();
	void DisplayLinePos(Line l);

private:
	uint8_t mmToLedPos(int16_t mm);
};
