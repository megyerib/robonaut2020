#include "System.h"
#include "DmaUart2.h"
#include <cstring>

int main(void)
{
	System::GetInstance();
	DmaUart2* uart = DmaUart2::GetInstance();
	static char msg[] = "Hello!\n";

	while (1)
	{
		uart->Send(msg, strlen(msg));
		HAL_Delay(1000);
	}
}

