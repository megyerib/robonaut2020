#pragma once

#include <QWidget>
#include "navimath2d.h"

class Display
{
public:
    QWidget&     widget;
    CartesianCS  cs;

    Display(QWidget& w);

    void SetCS(CartesianCS* cs);
    void Drag(QPoint &start, QPoint &end);
    void Zoom(QPoint &center, double magnitude);
};
