#pragma once

#include <stdint.h>
#include "RemoteHw.h"

typedef enum
{
	SteeringCh = 0,
	ThrottleCh,

	CH_Num
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

	bool CalibrationStart(RemoteChannel ch);
	bool IsCalibrationInProgress(RemoteChannel ch);
	void GetCalibrationValues(RemoteChannel ch, RemoteCal& cal);
	void SetCalibrationValues(RemoteChannel ch, RemoteCal& cal);

	void CalibrationProcess();

private:
	RemoteCal calData[CH_Num]  = {0};
	bool calInProgress[CH_Num] = {false};
	bool calStarted[CH_Num] = {false};
	RemoteHw* remoteHw;

	Remote();
};
