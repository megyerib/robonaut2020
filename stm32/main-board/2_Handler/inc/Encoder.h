#pragma once

class Encoder
{
public:
	static Encoder* GetInstance();
	float GetDistance(); /* m */
	float GetSpeed(); /* m/s */
	void Cycle();

private:
	Encoder();
};
