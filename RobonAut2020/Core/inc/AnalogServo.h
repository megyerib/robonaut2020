/*
 * AnalogServo.h
 *
 *  Created on: 2019. nov. 16.
 *      Author: Joci
 */

#ifndef ANALOGSERVO_H_
#define ANALOGSERVO_H_

#include "Servo.h"

class AnalogServo : public Servo
{
public:
    AnalogServo(TIM_HandleTypeDef* Htim, uint32_t Channel, ServoType Type, bool isChNegated = false);
    ~AnalogServo();

private:
};


#endif /* ANALOGSERVO_H_ */
