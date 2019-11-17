/*
 * DigitalServo.h
 *
 *  Created on: 2019. nov. 16.
 *      Author: Joci
 */

#ifndef DIGITALSERVO_H_
#define DIGITALSERVO_H_

#include "Servo.h"

class DigitalServo : public Servo
{
public:
    DigitalServo(TIM_HandleTypeDef* Htim, uint32_t Channel, ServoType Type, bool isChNegated = false);
    ~DigitalServo();

private:
};


#endif /* DIGITALSERVO_H_ */
