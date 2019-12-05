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

    tof->Init();
    servo_sensor->Enable();

    Pd_ctlr = Pd_Controller(0.5f, 0.1f);
    Pd_ctlr.SetSetpoint(0.52f);
}

void TestTask::TaskFunction()
{
	EncoderHw* enc = EncoderHw::GetInstance();

	uint32_t encval = enc->GetCounterValue();

	tof->Process();

// Servo Test
//    servo_sensor->SetSteerAngle(2.62f);
//    vTaskDelay(1000);
//    servo_sensor->SetSteerAngle(1.57f);
//    vTaskDelay(1000);
//    servo_sensor->SetSteerAngle(0.52f);
//    vTaskDelay(2000);

// PD Test
    Pd_ctlr.Process(servo_sensor->GetSteerAngle());
    servo_sensor->SetSteerAngle(servo_sensor->GetSteerAngle() + Pd_ctlr.GetControlValue());

}
