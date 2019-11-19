#pragma once

class System
{
public:
	static System* GetInstance();
	void OsStart();

private:
	System();
	void Clock_Config();
};
