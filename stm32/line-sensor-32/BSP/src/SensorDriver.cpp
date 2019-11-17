#include "SensorDriver.h"

SensorDriver::SensorDriver() : sensors(1, 11)
{

}

void SensorDriver::SetSensors(uint8_t groupSize, uint8_t index)
{
	uint32_t buffer = 0;

	// TODO smaller sensor size
	for (int i = 0; (i * groupSize + index) < SENSOR_SIZE; i++)
	{
		buffer |= 1 << (i * groupSize + index);
	}

	sensors.Display(&buffer, 4);
}
