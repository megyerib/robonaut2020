/*
 * Task_Servo.h
 *
 *  Created on: Nov 13, 2019
 *      Author: Joci
 */

#ifndef TASK_SERVO_H_
#define TASK_SERVO_H_

class Task_Servo
{
public:
    static void Init_Task_Servo();
    static void Task_Servo_Process(void *pvParameters);
};

#endif /* TASK_SERVO_H_ */
