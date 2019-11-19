#pragma once

#include <stdint.h>

typedef enum
{
	CH1,
	CH2,
	CH3,
	CH4
}
RemoteChannel;

typedef struct
{
	uint32_t min;
	uint32_t mid;
	uint32_t max;
}
RemoteCal;

class Remote
{
public:
	static Remote* GetInstance();
	float GetValue(RemoteChannel ch); /* [-1; +1] */

	void Calibrate(RemoteChannel ch);
	void GetCalibrationValues(RemoteChannel ch, RemoteCal& cal);
	void SetCalibrationValues(RemoteChannel ch, RemoteCal& cal);
};
