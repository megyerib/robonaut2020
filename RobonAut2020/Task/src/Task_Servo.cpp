/*
 * Task_Servo.cpp
 *
 *  Created on: Nov 13, 2019
 *      Author: Joci
 */

#include "Task_Servo.h"

#include "main.h"
#include "OVERRIDE_Types.h"

#include "FreeRTOS.h"
#include "task.h"

#include "AnalogServo.h"
#include "DigitalServo.h"
#include "Servo.h"
#include "Steer.h"
#include "Pd_Controller.h"


void Task_Servo::Init_Task_Servo()
{
    xTaskCreate(Task_Servo::Task_Servo_Process,
                "TASK_SERVO",
                512,
                NULL,
                5,
                NULL);
}

void Task_Servo::Task_Servo_Process(void *pvParameters)
{
    DigitalServo servo_front(&htim12, TIM_CHANNEL_1, ServoType::SRT_CH6012);
    DigitalServo servo_rear(&htim12,  TIM_CHANNEL_2, ServoType::SRT_CH6012);
    AnalogServo  servo_sensor(&htim8, TIM_CHANNEL_1, ServoType::FUTABA_S3003, true);

    // TODO steering test
    Steer wheel(&servo_front);
    // TEST END

    // TODO controller test
    Controller    P_ctrl;
    Pd_Controller Pd_ctlr(2.1f, 2.5f);

    P_ctrl.SetSetpoint(PI/2);
    Pd_ctlr.SetSetpoint(PI/2);
    // TEST END

    // TODO servo test
    servo_sensor.Enable();
    auto c = servo_sensor.GetSteerAngle();
    servo_sensor.SetSteerAngle(60.0f);
    c = servo_sensor.GetSteerAngle();

    while (1)
    {
        servo_sensor.SetSteerAngle(120.0f);
        vTaskDelay(1000);
        servo_sensor.SetSteerAngle(60.0f);

        P_ctrl.Process(servo_sensor.GetSteerAngle());
        Pd_ctlr.Process(servo_sensor.GetSteerAngle());

        vTaskDelay(2000);
    }
    // TEST END
}
