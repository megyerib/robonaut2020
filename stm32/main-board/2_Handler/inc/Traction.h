#pragma once

typedef enum
{
	Acceleration,
	Braking
	// ...
}
TractionMode;

class Traction
{
public:
	static Traction* GetInstance();
	void SetSpeed(float speed /* m/s */);
	void SetMode(TractionMode mode);
};
