#pragma once

#include "robotproxy.h"
#include "virtualrobot.h"
#include <QWidget>
#include <QTimer>
#include "sensor/linesensoravg.h"

class SimRobot1 : public VirtualRobot,  public RobotProxy
{
    CartesianPos* position;
    QImage* robotImg;

public:
    SimRobot1(Display* d, CartesianPos* initialPos);

    void SetSpeed(double speed) override;
    void SetSteering(double angle) override;

    void Draw() override;

private:
    void Refresh() override;


    CartesianCS* carCs;

    /*LineSensorAvg* lineSensor = nullptr;
    CartesianLoc* lsStart;
    CartesianLoc* lsEnd;*/

    double speed, targetSpeed;
    double steerAngle, targetSteerAngle;



    double prev_line; // TODO make a separate controller class for this!

    void CalcPosition();
    void CalcSpeed();
    void CalcSteer();

    void ConfigLineSensor();
    void UpdateLineSensor();
};
