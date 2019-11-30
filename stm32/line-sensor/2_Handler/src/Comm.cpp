#include "Comm.h"
#include "base64.h"

Comm::Comm()
{

}

Comm* Comm::GetInstance()
{
	static Comm instance;

	return &instance;
}

void Comm::SendLine(Line l)
{

}
