#include "simrobot1.h"
#include "navimath2d.h"
#include <QPainter>
#include "QDebug"

#define INITIAL_X 2.0
#define INITIAL_Y 1.0
#define INITIAL_PHI DEG_TO_RAD(-30)

#define ROBOT_LENGTH 0.4 /* [m] */
#define REFRESH_INTERVAL 20

SimRobot1::SimRobot1() :
    position(WORLD, INITIAL_X, INITIAL_Y, INITIAL_PHI),
    robotImg("car.png")
{
    // Set timer
    refreshTimer.setInterval(REFRESH_INTERVAL);
    connect(&refreshTimer, SIGNAL(timeout()), this, SLOT(Refresh()));
    refreshTimer.start();
}

void SimRobot1::SetSpeed(double speed)
{
    Q_UNUSED(speed)
}

void SimRobot1::SetSteering(double angle)
{
    Q_UNUSED(angle)
}

void SimRobot1::Draw(QWidget& w, CartesianCS* targetCS)
{
    //  +---> width, length
    //  | [CAR>
    //  V height, width

    QPainter painter(&w);

    CartesianPos carPos(position);
    carPos.TransformTo(targetCS);

    int carLenPx = int(ROBOT_LENGTH / abs(targetCS->x_res));
    int carWidthPx = carLenPx * robotImg.height() / robotImg.width(); // See comment

    QRect source(0, 0, robotImg.width(), robotImg.height());
    QRect target(-carLenPx/2, -carWidthPx/2, carLenPx, carWidthPx);

    painter.translate(carPos.GetX(), carPos.GetY());
    painter.rotate(RAD_TO_DEG(carPos.GetPhi()));

    painter.drawImage(target, robotImg, source);
}

void SimRobot1::Refresh()
{

}
