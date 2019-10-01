#ifndef SIMROBOT1_H
#define SIMROBOT1_H

#include "robotproxy.h"
#include "virtualrobot.h"
#include <QWidget>
#include <QTimer>
#include "linesensoravg.h"

class SimRobot1 : public QObject, public RobotProxy, public VirtualRobot
{
    Q_OBJECT

public:
    SimRobot1();

    void SetSpeed(double speed) override;
    void SetSteering(double angle) override;

    void Draw() override;
    void PostCfg() override;

private:
    CartesianPos position;
    QImage robotImg;
    CartesianCS carCs;

    LineSensorAvg* lineSensor = nullptr;
    CartesianLoc* lsStart;
    CartesianLoc* lsEnd;

    double speed, targetSpeed;
    double steerAngle, targetSteerAngle;

    QTimer refreshTimer;

    double prev_line; // TODO make a separate controller class for this!

    void CalcPosition();
    void CalcSpeed();
    void CalcSteer();

    void ConfigLineSensor();
    void UpdateLineSensor();

private slots:
    void Refresh();
};

#endif // SIMROBOT1_H
