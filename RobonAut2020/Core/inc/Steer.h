/*
 * Steer.h
 *
 *  Created on: 2019. nov. 11.
 *      Author: Joci
 */

#ifndef STEER_H_
#define STEER_H_

#include "ISteer.h"

class Steer
{
private:
    ISteer* wheel_front;
    ISteer* wheel_rear;

public:
    Steer(){};
    Steer(ISteer* wh_front)
    {
        wheel_front = wh_front;
        wheel_rear  = NULL;
    }
    Steer(ISteer* wh_front, ISteer* wh_rear)
    {
        wheel_front = wh_front;
        wheel_rear  = wh_rear;
    }
    ~Steer() {};

    void Enable(void)
    {
        wheel_front->Enable();
    }

    void Disable(void)
    {
        wheel_front->Disable();
    }

    //! Steering wheel:
    //!
    //!               0deg
    //!        +30deg  |   -30deg
    //!            \   |   /
    //!             \  |  /
    //!              \ | /
    //!    +90________\|/________-90deg
    //!  Left end              Right end
    void rotateWheel(float angle)
    {
        wheel_front->SetSteerAngle(angle);
    }

    float getWheelAngle(void)
    {
        return wheel_front->GetSteerAngle();
    }
};

#endif /* STEER_H_ */
