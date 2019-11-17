#pragma once

#include "LedDriver.h"

class SensorDriver
{
	LedDriver sensors;

public:
	SensorDriver();
	void SetSensors(uint8_t groupSize, uint8_t index);
};
