#pragma once

typedef enum
{
	Single,
	Acceleration,
	Braking
}
TrackType;

typedef enum
{
	Maze,
	Speedrun
}
TrackMode;

class TrackDetector
{
public:
	static TrackDetector* GetInstance();
	void SetMode(TrackMode mode);
	TrackType GetTrackType();
	float GetFrontLine(); /* m */
	float GetRearLine(); /* m */
};
