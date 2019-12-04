#include "TestTask.h"
#include "EncoderHw.h"


TestTask::TestTask() : CyclicTask((char*)"TEST", 20, 1, 512)
{

}

TestTask* TestTask::Init()
{
	static TestTask instance;

	return &instance;
}

void TestTask::TaskInit()
{
    tof = new TOF_L1();
    servo_sensor = new Servo(eTIM8, TIM_CHANNEL_1);

    //tof->Init();
    servo_sensor->Enable();
}

void TestTask::TaskFunction()
{
	EncoderHw* enc = EncoderHw::GetInstance();

	uint32_t encval = enc->GetCounterValue();

	tof->Process();

    servo_sensor->SetSteerAngle(2.62f);
    vTaskDelay(1000);
    servo_sensor->SetSteerAngle(1.57f);
    vTaskDelay(1000);
    servo_sensor->SetSteerAngle(0.52f);
    vTaskDelay(2000);
}
