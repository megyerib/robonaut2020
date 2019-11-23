/*
 * Servo.h
 *
 *  Created on: 2019. nov. 11.
 *      Author: Joci
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "OVERRIDE_Types.h"
#include "ISteer.h"
#include "Timer.h"

enum ServoType
{
    UNKNOWN = 0,
    FUTABA_S3003,
    MAVERICK_MS22,
    SRT_CH6012
};

struct ServoConfig
{
    U16   Deg_min;
    U16   Deg_30;
    U16   Deg_90;
    U16   Deg_150;
    U16   Deg_max;
    float Gradient;
    float Y_intercept;
};

class Servo : public ISteer
{
protected:
     ServoType   type;
     bool        enabled;
     ServoConfig config;
     Timer*      timer;

public:
     Servo();
     virtual ~Servo() = 0;

     void  Enable(void);
     void  Disable(void);
     void  SetSteerAngle(float rad);
     float GetSteerAngle(void);

     void SetCalibration(U8 min,
                         U8 rad30,
                         U8 rad90,
                         U8 rad150,
                         U8 max);
};

#endif /* SERVO_H_ */
