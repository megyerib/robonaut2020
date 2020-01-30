/*
 * ISteer.h
 *
 *  Created on: 2019. nov. 11.
 *      Author: Joci
 */

#ifndef ISTEER_H_
#define ISTEER_H_


class ISteer
{
public:
    virtual ~ISteer() {};
    virtual void  Enable        (void) = 0;
    virtual void  Disable       (void) = 0;
    virtual void  SetSteerAngle (float rad) = 0;
    virtual float GetSteerAngle (void) = 0;
};


#endif /* ISTEER_H_ */
