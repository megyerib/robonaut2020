/*
 * Car.cpp
 *
 *  Created on: Nov 17, 2019
 *      Author: Joci
 */

#include "Car.h"

Car::Car(void)
{
    state = RobotState::Init;


    state = RobotState::Ideal;
}

void Car::Process_Quali(void)
{
    switch (state)
    {
        case RobotState::Init:
        {
            // Initializing SW components.
            if ("everything is initialized")
            {
                state = RobotState::Ideal;
            }
            break;
        }
        case RobotState::Ideal:
        {
            // Car is waiting for remote start.
            break;
        }
        case RobotState::Follow:
        {
            // Line follow controller
            // Distance controller
            // Speed adjustment
            break;
        }
        case RobotState::LineLost:
        {
            // Keep the wheel, to get back to the line
            break;
        }
        case RobotState::Overtake:
        {
            // Do the maneuver
            break;
        }
        case RobotState::Stop:
        {
            // Remote stopped the car
            break;
        }
        default:
        {
            break;
        }
    }
}

