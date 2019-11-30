#include "TestTask.h"
#include "ExtUart3.h"
#include "Line.h"
#include "base64.h"

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
	uart = ExtUart3::GetInstance();
}

void TestTask::TaskFunction()
{
	uint8_t base64_buf[30];
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
	}
}
