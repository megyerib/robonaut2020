#pragma once

#include "robot.h"
#include "virtualrobot.h"
#include <QWidget>
#include <QTimer>
#include "sensor/linesensoravg.h"

class SimRobot1 : public VirtualRobot,  public Robot
{
    Track* track;
    CartesianCS* carCs;
    CartesianPos* position;
    QImage* robotImg;

    LineSensorAvg* lineSensor;
    CartesianLoc* lsStart;
    CartesianLoc* lsEnd;

public:
    SimRobot1(Display* d, Track* t, CartesianPos* initialPos);

    CartesianPos GetRealPosition() override;

    void SetSpeed(double speed) override;
    void SetSteering(double angle) override;

    void Draw() override;

private:
    void Refresh() override;

    double speed = 0, targetSpeed;
    double steerAngle = 0, targetSteerAngle;

    double prev_line; // TODO make a separate controller class for this!

    void CalcPosition();
    void CalcSpeed();
    void CalcSteer();

    void ConfigLineSensor();
};
