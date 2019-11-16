#pragma once

class Rcc
{
public:
	static Rcc* Init();

private:
	Rcc();
	void SystemClock_Config();
};
