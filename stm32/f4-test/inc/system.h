#pragma once

class System
{
private:
	System();

	void Clock_Config();

public:
	static System* Get();

	void OsStart();
};
