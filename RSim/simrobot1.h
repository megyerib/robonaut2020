#ifndef SIMROBOT1_H
#define SIMROBOT1_H

#include "robotproxy.h"
#include "virtualrobot.h"
#include <QWidget>
#include <QTimer>

class SimRobot1 : public QObject, public RobotProxy, public VirtualRobot
{
    Q_OBJECT

    CartesianPos position;
    QImage robotImg;
    double speed;
    double steerAngle;

    QTimer refreshTimer;

public:
    SimRobot1();

    void SetSpeed(double speed) override;
    void SetSteering(double angle) override;

    void Draw() override;

private slots:
    void Refresh();
};

#endif // SIMROBOT1_H
