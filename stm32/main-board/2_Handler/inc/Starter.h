#pragma once

typedef enum
{
	Go,
	CountDown1,
	CountDown2,
	CountDown3,
	CountDown4,
	CountDown5,
	NoSignal
}
StarterState;

class Starter
{
public:
	static Starter* GetInstance();
	StarterState GetState();
};
