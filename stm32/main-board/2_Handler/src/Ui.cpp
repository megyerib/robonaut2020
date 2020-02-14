#include "Ui.h"

Ui::Ui()
{
	uart = UiUart::GetInstance();
}

Ui* Ui::GetInstance()
{
	static Ui instance;
	return &instance;
}

bool Ui::IsStopped()
{
	// TODO
	return false;
}

bool Ui::GetCommand(uint8_t* cmd)
{
	// TODO
	return 0;
}

void Ui::SetCommand(uint8_t command)
{
	UNUSED(command);
	// TODO
}
