#pragma once

typedef enum
{
	DualLineFollow,
	SingleLineFollow,
	Free,
	Reverse
	// ...
}
SteeringMode;

class Steering
{
public:
	static Steering* GetInstance();
	void SetMode(SteeringMode mode);
	void SetLine(float front /* m */, float rear /* m */); // For LineFollow modes
	void SetAngle(float front /* rad */, float rear /* rad */); // For Free mode
};
