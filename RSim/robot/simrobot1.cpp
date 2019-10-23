#include "simrobot1.h"
#include "navimath2d.h"
#include <QPainter>
#include <cstdlib>

#define ROBOT_LENGTH 0.4 /* [m] */
#define REFRESH_INTERVAL 0.05 /* [s] */

#define WHEELBASE 0.4 /* [m] Axle to axle distance */
#define ACCELERATION 2.0 /* [m/s^2] */
#define MAX_SPEED 10.0
#define STEER_SPEED DEG_TO_RAD(350) /* [rad/s] */
#define MAX_STEER DEG_TO_RAD(45) /* [rad] */

SimRobot1::SimRobot1(Display* d, Track *t, CartesianPos* initialPos)
{
    display = d;
    track   = t;

    position = new CartesianPos(*initialPos);

    robotImg = new QImage("../RSim/resource/car_red.png");

    carCs = new CartesianCS(position->x(), position->y(), position->phi(), 1, 1);

    ConfigLineSensor();

    InitRefreshTimer(REFRESH_INTERVAL);

    SetSpeed(1.5);
}

void SimRobot1::Draw()
{
    //  +---> length  +---> width
    //  | [CAR>       | [Display]
    //  V width       V height

    QPainter painter(&display->widget);

    CartesianPos carPos(*position);
    carPos.TransformTo(&display->cs);

    int carLenPx = int(ROBOT_LENGTH / abs(display->cs.x_res));
    int carWidthPx = carLenPx * robotImg->height() / robotImg->width(); // See comment

    QRect source(0, 0, robotImg->width(), robotImg->height());
    QRect target(-carLenPx/2, -carWidthPx/2, carLenPx, carWidthPx);

    painter.translate(carPos.x(), carPos.y());
    painter.rotate(-RAD_TO_DEG(carPos.phi())); // Clockwise; degrees

    painter.drawImage(target, *robotImg, source);

    lineSensor->Draw();
}

void SimRobot1::SetSpeed(double speed)
{
    targetSpeed = speed;
}

void SimRobot1::SetSteering(double angle)
{
    targetSteerAngle = angle;
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

    position->SetX(position->x() + s * cos(position->phi()));
    position->SetY(position->y() + s * sin(position->phi()));
    position->SetPhi(position->phi() + sign * beta);

    carCs->center_x = position->x();
    carCs->center_y = position->y();
    carCs->alpha = position->phi();
    carCs->RecalcBaseVectors();
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

    double P = 3.5 * line / -0.15 * DEG_TO_RAD(60);
    double D = 1.5 * (line - prev_line) / -0.15 * DEG_TO_RAD(60);
    prev_line = line;

    SetSteering(P + D);

    CalcSpeed();
    CalcSteer();

    CalcPosition();
}

void SimRobot1::ConfigLineSensor()
{
    lsStart = new CartesianLoc(carCs, WHEELBASE/2,  0.15);
    lsEnd   = new CartesianLoc(carCs, WHEELBASE/2, -0.15);
    lineSensor = new LineSensorAvg(display, track, lsStart, lsEnd);
}

CartesianPos SimRobot1::GetRealPosition()
{
    return *position;
}
