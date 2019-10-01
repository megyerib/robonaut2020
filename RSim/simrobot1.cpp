#include "simrobot1.h"
#include "navimath2d.h"
#include <QPainter>
#include "QDebug"
#include "SimRobot1Params.h"

SimRobot1::SimRobot1() :
    position(WORLD, INITIAL_X, INITIAL_Y, INITIAL_PHI),
    robotImg("../RSim/resource/car_red.png"),
    carCs(INITIAL_X, INITIAL_Y, INITIAL_PHI, 1, 1)
{
    // Set timer
    refreshTimer.setInterval(int(REFRESH_INTERVAL*1000));
    connect(&refreshTimer, SIGNAL(timeout()), this, SLOT(Refresh()));
    refreshTimer.start();

    // Initial values
    SetSpeed(1);
    SetSteering(DEG_TO_RAD(0));

    // Configure line sensor
    ConfigLineSensor();
}

void SimRobot1::SetSpeed(double speed)
{
    targetSpeed = speed;
}

void SimRobot1::SetSteering(double angle)
{
    targetSteerAngle = angle;
}

void SimRobot1::Draw()
{
    //  +---> width, length
    //  | [CAR>
    //  V height, width

    QPainter painter(bgWidget);

    CartesianPos carPos(position);
    carPos.TransformTo(windowCS);

    int carLenPx = int(ROBOT_LENGTH / abs(windowCS->x_res));
    int carWidthPx = carLenPx * robotImg.height() / robotImg.width(); // See comment

    QRect source(0, 0, robotImg.width(), robotImg.height());
    QRect target(-carLenPx/2, -carWidthPx/2, carLenPx, carWidthPx);

    painter.translate(carPos.GetX(), carPos.GetY());
    painter.rotate(-RAD_TO_DEG(carPos.GetPhi())); // Clockwise; degrees

    painter.drawImage(target, robotImg, source);

    // Sensor draw
    QPainter sensorPainter(bgWidget);

    CartesianLoc sstart(*lineSensor->startLoc);
    sstart.TransformTo(windowCS);
    CartesianLoc send(*lineSensor->endLoc);
    send.TransformTo(windowCS);

    sensorPainter.drawLine(int(sstart.GetX()), int(sstart.GetY()), int(send.GetX()), int(send.GetY()));
}

void SimRobot1::CalcPosition()
{
    double sign = steerAngle > 0 ? 1.0 : -1.0;
    double alpha = steerAngle * sign; // abs
    double R_rear = WHEELBASE / tan(alpha);
    double h = WHEELBASE / 2;
    double gamma = atan(h/R_rear);
    double R_mid = R_rear / cos(gamma);
    double beta = speed * REFRESH_INTERVAL / R_mid;

    double s = speed * REFRESH_INTERVAL;

    position.SetX(position.GetX() + s * cos(position.GetPhi()));
    position.SetY(position.GetY() + s * sin(position.GetPhi()));
    position.SetPhi(position.GetPhi() + sign * beta);

    carCs.center_x = position.GetX();
    carCs.center_y = position.GetY();
    carCs.alpha = position.GetPhi();
    carCs.RecalcBaseVectors();
}

void SimRobot1::CalcSpeed()
{
    if (targetSpeed > speed)
    {
        speed += ACCELERATION * REFRESH_INTERVAL;
        if (speed > targetSpeed)
            speed = targetSpeed;
    }
    else
    {
        speed -= ACCELERATION * REFRESH_INTERVAL;
        if (speed < targetSpeed)
            speed = targetSpeed;
    }

    if (speed > MAX_SPEED)
        speed = MAX_SPEED;
    else if (speed < -MAX_SPEED)
        speed = -MAX_SPEED;
}

void SimRobot1::CalcSteer()
{
    if (targetSteerAngle > steerAngle)
    {
        steerAngle += STEER_SPEED * REFRESH_INTERVAL;
        if (steerAngle > targetSteerAngle)
            steerAngle = targetSteerAngle;
    }
    else
    {
        steerAngle -= STEER_SPEED * REFRESH_INTERVAL;
        if (steerAngle < targetSteerAngle)
            steerAngle = targetSteerAngle;
    }

    if (steerAngle > MAX_STEER)
        steerAngle = MAX_STEER;
    else if (steerAngle < -MAX_STEER)
        steerAngle = -MAX_STEER;
}

void SimRobot1::Refresh()
{
    double line = lineSensor->getLine();

    double P = 0.5 * line / -0.15 * DEG_TO_RAD(60);
    double D = 0.0 * (line - prev_line) / -0.15 * DEG_TO_RAD(60);
    prev_line = line;

    steerAngle = P + D;
    //SetSteering(P + D);

    CalcSpeed();
    //CalcSteer();
    CalcPosition();
}

void SimRobot1::ConfigLineSensor()
{
    lsStart = new CartesianLoc(&carCs, WHEELBASE/2,  0.15);
    lsEnd   = new CartesianLoc(&carCs, WHEELBASE/2, -0.15);

    lineSensor = new LineSensorAvg(lsStart, lsEnd, bgImg, bgCS);
}

void SimRobot1::PostCfg()
{
    ConfigLineSensor();
}
