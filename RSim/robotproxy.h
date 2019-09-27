#pragma once

class RobotProxy
{
    // No variables
    // This abstract class can only contain interface functions.
    // For simulator related functions see VirtualRobot class

public:
    virtual ~RobotProxy() = default;

    virtual void SetSpeed(double speed) = 0;
    virtual void SetSteering(double angle) = 0;
};
