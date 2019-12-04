#pragma once

#include "CyclicTask.h"
#include "ToF_4m_L1.h"
#include "Servo.h"

class TestTask : public CyclicTask
{
private:
    TOF_L1* tof;
    Servo* servo_front;
    Servo* servo_rear;
    Servo* servo_sensor;

	TestTask();

	void TaskInit() override;
	void TaskFunction() override;

public:
	static TestTask* Init();
};
