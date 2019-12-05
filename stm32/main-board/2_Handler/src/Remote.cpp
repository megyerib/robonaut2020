#include "Remote.h"

#define CALIB_THRESHOLD 50
#define ABS(x) (((x) < 0) ? -(x) : (x))

const RemoteHwChannel chTable[CH_Num] =
{
	[Steering] = RemCh1,
	[Throttle] = RemCh2
};

Remote::Remote()
{
	remoteHw = RemoteHw::GetInstance();
}

Remote* Remote::GetInstance()
{
	static Remote instance;
	return &instance;
}

float Remote::GetValue(RemoteChannel ch)
{
	// TODO
	return 0;
}

void Remote::CalibrationStart(RemoteChannel ch)
{
	calInProgress[ch] = true;
}

bool Remote::IsCalibrationInProgress(RemoteChannel ch)
{
	return calInProgress[ch];
	return calStarted[ch];
}

void Remote::GetCalibrationValues(RemoteChannel ch, RemoteCal& cal)
{

}

void Remote::SetCalibrationValues(RemoteChannel ch, RemoteCal& cal)
{

}

void Remote::CalibrationProcess()
{
	static RemoteCal tmpData[CH_Num];

	for (int ch = 0; ch < CH_Num; ch++)
	{
		if (calInProgress[ch])
		{
			uint16_t pulse = remoteHw->GetPulseWidth(chTable[ch]);

			if (calStarted[ch])
			{
				tmpData[ch] = {0, 0, 0};

				tmpData[ch].mid = pulse;

				calStarted[ch] = false;
			}
			else
			{
				if (pulse > (tmpData[ch].mid + CALIB_THRESHOLD) && pulse > tmpData[ch].max)
				{
					tmpData[ch].max = pulse;
				}

				if (pulse < (tmpData[ch].mid - CALIB_THRESHOLD) && pulse < tmpData[ch].min)
				{
					tmpData[ch].min = pulse;
				}

				if (tmpData[ch].min > 0 && tmpData[ch].max > 0 && ABS(tmpData[ch].mid - pulse) < CALIB_THRESHOLD)
				{
					calData[ch] = tmpData[ch];
					calInProgress[ch] = false;
				}
			}
		}
	}
}
