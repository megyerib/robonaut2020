#include "Traction.h"
#include "printf.h" /* Lightweight printf */

Traction::Traction()
{
	uart = MotorUart::GetInstance();
}

Traction* Traction::GetInstance()
{
	static Traction instance;
	return &instance;
}

void Traction::SetSpeed(float speed /* m/s */)
{
	targetSpeed = speed;
}

void Traction::SetMode(TractionMode mode)
{
	this->mode = mode;
}

void Traction::SetDutyCycle(float d /* % [-100;+100] */)
{
	uint8_t buf[10];

	int message = d * 1000;
	int size;

	size = sprintf((char*)buf, "%d\r\n", message);

	uart->Send(buf, size);
}

void Traction::Process()
{
	SetDutyCycle(20.5); // TODO
}
