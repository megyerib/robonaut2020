#include "checkpoint.h"
#include <QPainter>

#define REFRESH_RATE 0.1
#define GATE_WIDTH 1.0
#define GATE_LENGTH 0.2

#define DISTANCE_THRESHOLD 0.2

Checkpoint::Checkpoint(Display& display, CartesianPos& pos, VirtualRobot &robot) :
    display(display),
    pos(pos),
    robot(robot)
{
    InitRefreshTimer(REFRESH_RATE);
}

void Checkpoint::Draw()
{
    QPainter painter(&display.widget);

    CartesianPos gatePos(pos);
    gatePos.TransformTo(&display.cs);

    int gateLenPx = int(GATE_LENGTH / abs(display.cs.x_res));
    int gateWidthPx = int(gateLenPx * GATE_WIDTH / GATE_LENGTH);

    QRect gate(-gateWidthPx/2,
               -gateLenPx/2,
               gateWidthPx,
               gateLenPx);

    painter.translate(gatePos.x(), gatePos.y());
    painter.rotate(-RAD_TO_DEG(gatePos.phi())); // Clockwise; degrees

    painter.setBrush(visited ? QColor(0,255,0,128) : QColor(255,0,0,128));
    painter.drawRect(gate);
}

void Checkpoint::Refresh()
{
    CartesianPos robotPos = robot.GetRealPosition();

    double x_diff = robotPos.x() - pos.x();
    double y_diff = robotPos.y() - pos.y();
    double dist   = sqrt(x_diff*x_diff+y_diff*y_diff);

    if (dist < DISTANCE_THRESHOLD)
        visited = true;
}
