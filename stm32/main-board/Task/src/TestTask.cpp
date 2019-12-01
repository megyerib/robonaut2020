#include "TestTask.h"
#include "Line.h"
#include "StarterUart.h"

extern "C"
{
#include "base64.h"
}

TestTask::TestTask() : CyclicTask((char*)"TEST", 500, 1, configMINIMAL_STACK_SIZE)
{

}

TestTask* TestTask::Init()
{
	static TestTask instance;

	return &instance;
}

void TestTask::TaskInit()
{
	uart = MotorUart::GetInstance();
}

void TestTask::TaskFunction()
{
	uart->Send((uint8_t*)"42069\r\n", 7);

	/*uint8_t base64_buf[30];
	size_t  base64_size;

	uint8_t decoded_buf[30];
	size_t  decoded_size;

	Line l;

	uart->GetMessage(base64_buf, &base64_size);

	if (base64_size > 0)
	{
		base64_decode(base64_buf, decoded_buf, base64_size, &decoded_size);

		if (decoded_size == sizeof(Line))
		{
			l = *((Line*) decoded_buf);
			(void) l;
		}
	}*/
}
