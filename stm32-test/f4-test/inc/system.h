#pragma once

class System
{
private:
	System();

	void Clock_Config();

public:
	static System* GetInstance();

	void OsStart();
};
