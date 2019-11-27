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

#define TEST_CASE     (3U)      //!< 0: no test, 1: Servo test, 2: PD test, 3: Steering test


void Task_Servo::Init_Task_Servo()
{
    xTaskCreate(Task_Servo::Task_Servo_Process,
                "TASK_SERVO",
                2024,
                NULL,
                6,
                NULL);
}

void Task_Servo::Task_Servo_Process(void *pvParameters)
{
    DigitalServo servo_front(&htim12, TIM_CHANNEL_1, ServoType::SRT_CH6012);
    DigitalServo servo_rear(&htim12,  TIM_CHANNEL_2, ServoType::SRT_CH6012);
    AnalogServo  servo_sensor(&htim8, TIM_CHANNEL_1, ServoType::FUTABA_S3003, true);

    Steer wheel(&servo_sensor);

#if 1
    // mini servo
    servo_sensor.SetCalibration(35, 44, 89, 134, 150);
#else
    // Futaba
    servo_sensor.SetCalibration(25, 49, 85, 121, 145);
#endif

    servo_sensor.Enable();
    servo_sensor.SetSteerAngle(2.62f);

#if TEST_CASE == 2
    Pd_Controller Pd_ctlr(0.5f, 0.1f);
    Pd_ctlr.SetSetpoint(0.52f);
#elif TEST_CASE == 3
    wheel.Enable();
#endif

    while (1)
    {
#if TEST_CASE == 1
        servo_sensor.SetSteerAngle(2.62f);
        vTaskDelay(1000);
        servo_sensor.SetSteerAngle(1.57f);
        vTaskDelay(1000);
        servo_sensor.SetSteerAngle(0.52f);
        vTaskDelay(2000);

#elif TEST_CASE == 2
        Pd_ctlr.Process(servo_sensor.GetSteerAngle());
        servo_sensor.SetSteerAngle(servo_sensor.GetSteerAngle()+Pd_ctlr.GetControlValue());
        vTaskDelay(2000);

#elif TEST_CASE == 3
        wheel.rotateWheel(1.57f);
        vTaskDelay(1000);
        wheel.rotateWheel(0.0f);
        vTaskDelay(1000);
        wheel.rotateWheel(-1.57f);
        vTaskDelay(2000);
#endif
    }
    // TEST END
}
