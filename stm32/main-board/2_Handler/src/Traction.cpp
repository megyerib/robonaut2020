#include "Traction.h"
#include "printf.h" /* Lightweight printf */

Traction::Traction()
{
	uart = MotorUart::GetInstance();
	encoder = Encoder::GetInstance();

	targetDutyCycle = 0;
}

Traction* Traction::GetInstance()
{
	static Traction instance;
	return &instance;
}

void Traction::SetSpeed(float speed /* m/s */)
{
	//targetSpeed = speed; // TODO
}

void Traction::SetMode(TractionMode mode)
{
	this->mode = mode;
}

void Traction::SetDutyCycle(float d /* % [-1;+1] */)
{
	targetDutyCycle = d;
}

void Traction::SendDutyCycle(float d /* % [-1;+1] */)
{
	uint8_t buf[10];

	int message = d * 100000;
	int size;

	size = sprintf((char*)buf, "%d\r\n", message);

	uart->Send(buf, size);
}

void Traction::Process()
{
	// TODO
	SendDutyCycle(targetDutyCycle);
}
