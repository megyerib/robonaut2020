#include "StddevEval.h"
#include <string.h>
#include <math.h>

#define THRESHOLD         200
#define SENSOR_NUM         32

#define CROSS_MIN_HIGHCNT   5
#define CROSS_MIN_LINECNT   4

StddevEval::StddevEval()
{

}

void StddevEval::Feed(AdcMeasType meas[SENSOR_SIZE])
{
	memcpy(data, meas, sizeof(data));
}

Line StddevEval::GetLine()
{
	uint32_t filtered[32];
	uint32_t i;
	uint32_t avg;
	uint32_t stdDev;
	uint32_t lineCnt = 0;

	Line ret;

	// Filtering
	removeSystematicError();
	magicDiff(data, filtered);

	// Average, standard deviation
	avg = mean(filtered, 32);
	stdDev = standardDeviation(filtered, 32, avg);

	// Sensor line
	for (i = 0; i < 32; i++)
	{
		if (evalIsPeak(filtered, i, avg, stdDev))
		{
			if (lineCnt < MAXLINES)
			{
				ret.lines[lineCnt] = evalWeightedMean(filtered, i);
			}

			lineCnt++;
		}
	}

	// Count
	ret.cnt = lineCnt > MAXLINES ? MAXLINES : lineCnt;

	// TODO Cross
	/*if (evalIsCross(filtered, avg + stdDev) || lineCnt > CROSS_MIN_LINECNT)
	{
		ret.cross = 1;
	}*/
	ret.cross = 0;

	// Return
	return ret;
}

// A 4 legközelebbi szomszédtól vett legnagyobb különbség, ha pozitív, egyébként 0.
void StddevEval::magicDiff(uint32_t* src, uint32_t* dst)
{
    int i;

    dst[0] = max4_pos(
        0,
        0,
        src[0] - src[1],
        src[0] - src[2]
    );

    dst[1] = max4_pos(
        0,
        src[1] - src[1],
        src[1] - src[2],
        src[1] - src[3]
    );

    for (i = 2; i < 30; i++)
    {
        dst[i] = max4_pos(
            src[i] - src[i-2],
            src[i] - src[i-1],
            src[i] - src[i+1],
            src[i] - src[i+2]
        );
    }

    dst[30] = max4_pos(
        src[30] - src[28],
        src[30] - src[29],
        src[30] - src[31],
        0
    );

    dst[31] = max4_pos(
        src[31] - src[29],
        src[31] - src[30],
        0,
        0
    );
}

// A 4 érték közül a legnagyobbal tér vissza, ha negatív, akkor 0-val.
uint32_t StddevEval::max4_pos(int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4)
{
    int32_t max = 0;

    max = arg1 > max ? arg1 : max;
    max = arg2 > max ? arg2 : max;
    max = arg3 > max ? arg3 : max;
    max = arg4 > max ? arg4 : max;

    return (uint32_t) max;
}

int16_t StddevEval::ledPosToMm(uint8_t ledPos)
{
    int16_t mm = (ledPos * IR_DIST_MM) - MID_IR_POS_MM;
    return mm;
}

int32_t StddevEval::evalWeightedMean(uint32_t arr[SENSOR_NUM], uint32_t i) // TODO itt a hiba
{
    int32_t w1, w2, w3, div, ret;

    if (i == 0)
    {
       w1 = 0;
       w2 = arr[0] * ledPosToMm(0);
       w3 = arr[1] * ledPosToMm(1);

       div = arr[0] + arr[1];
    }
    else if (i == 31)
    {
        w1 = arr[30] * ledPosToMm(30);
        w2 = arr[31] * ledPosToMm(31);
        w3 = 0;

        div = arr[30] + arr[31];
    }
    else
    {
        w1 = arr[i-1] * ledPosToMm(i-1);
        w2 = arr[i  ] * ledPosToMm(i  );
        w3 = arr[i+1] * ledPosToMm(i+1);

        div = arr[i-1] + arr[i] + arr[i+1];
    }

    ret = (w1 + w2 + w3) / div;

    return ret;
}

uint32_t StddevEval::evalIsPeak(uint32_t* arr, uint32_t i, uint32_t mean, uint32_t stdDev)
{
    // Threshold
    if (arr[i] < mean + stdDev)
    {
        return 0;
    }

    // Peak
    if (i == 0)
    {
        return (arr[1]  + THRESHOLD) < arr[0];
    }
    else if (i == 31)
    {
        return (arr[30] + THRESHOLD) < arr[31];
    }
    else
    {
        return
            ( ((arr[i-1] + THRESHOLD) < arr[i]) && (arr[i+1] < arr[i]) )
            ||
            ( ((arr[i+1] + THRESHOLD) < arr[i]) && (arr[i-1] < arr[i]) );
    }
}

int StddevEval::evalIsCross(uint32_t* arr, uint32_t threshold)
{
	int highCount = 0;
	int ret = 0;

	for (int i = 0; i < SENSOR_NUM; i++)
	{
		if (arr[i] < threshold)
		{
			highCount = 0;
		}
		else
		{
			highCount++;

			if (highCount >= CROSS_MIN_HIGHCNT)
				ret = 1;
		}
	}

	return ret;
}

uint32_t StddevEval::mean(uint32_t* data, uint32_t num)
{
    uint32_t sum = 0, i;

    for (i = 0; i < num; i++)
        sum += data[i];

    return sum / num;
}

uint32_t StddevEval::standardDeviation(uint32_t* data, uint32_t num, uint32_t avg)
{
    uint32_t sqsum = 0, var, i;

    for (i = 0; i < num; i++)
        sqsum += (data[i] - avg) * (data[i] - avg);

    var = sqsum / num;

    return sqrt(var);
}

void StddevEval::removeSystematicError()
{
	const int32_t correctionData[SENSOR_SIZE] =
	{-31, -24, -29, -21, -26, -32, -30, -11, -9, -8, -1, -2, -18, -7, 12, 16, 32, 34, 26, 36, 29, 22, 33};

	for (int i = 0; i < SENSOR_SIZE; i++)
	{
		data[i] -= correctionData[i];
	}
}
