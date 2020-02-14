#pragma once

#include "UiUart.h"

class Ui
{
public:
	static Ui* GetInstance();

	bool IsStopped();
	bool GetCommand(uint8_t* cmd);
	void SetCommand(uint8_t command);

private:
	UiUart* uart;
	Ui();
};
