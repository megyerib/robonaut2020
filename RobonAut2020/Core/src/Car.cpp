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
}

void Car::Process_Quali(void)
{
    // Common todos

    // State specific todos
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
            if (remote.GetState(TIM_CHANNEL_1) == 1)
            {
                state = RobotState::Follow;
            }
            break;
        }
        case RobotState::Follow:
        {
            // Line follow controller
            /// Get line
            lineFollowController.Process(1.0f);
            wheel.rotateWheel(lineFollowController.GetControlValue());

            // Distance controller
            frontDistanceSensor.Process();
            distanceController.Process(frontDistanceSensor.GetDistance_mm());
            /// Motor speed

            if (0 && "Nem talált vonalat")
            {
                state = RobotState::LineLost;
            }
            break;
        }
        case RobotState::LineLost:
        {
            // Keep the wheel, to get back to the line

            if ("Megvan a vonal")
            {
                state = RobotState::Follow;
            }
            break;
        }
        case RobotState::Overtake:
        {
            // Do the maneuver

            // Feltétel, hogy elég közel van a kocsi és a megfelelő kanyarban vagyunk
            // IF (feltétel == true)
            // {
            //      kikanyarodás
            //      párhuzamosba állás a vonallal
            //      gyorsítás
            //      IF (elhaladtunk mellette)
            //      {
            //          tovább megyünk
            //          vissza a vonal felé
            //          lassítunk
            //      }
            //      ELSE
            //      {
            //          haladunk amíg van még út
            //          ha nincs lassítás
            //          visszakanyarodás
            //          safety car követés
            //      }
            // }

            if ("Sikeres előzés")
            {
                state = RobotState::Follow;
            }
            else
            {
                // Gyorsabb kör
            }
            break;
        }
        case RobotState::Stop:
        {
            // Remote stopped the car
            /// Motor állj

            if ("remote újra engedélyez")
            {

            }
            break;
        }
        default:
        {
            break;
        }
    }
}

