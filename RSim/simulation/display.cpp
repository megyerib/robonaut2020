#include "display.h"

Display::Display(QWidget& w) : widget(w), cs(0, 0, 0, 1, 1)
{

}

void Display::SetCS(CartesianCS* cs)
{
    this->cs = *cs;
}

void Display::Drag(QPoint &start, QPoint &end)
{
    CartesianLoc move(&cs, -(end.x() - start.x()), -(end.y() - start.y())); // New midpoint
    move.TransformTo(WORLD);

    cs.center_x = move.x();
    cs.center_y = move.y();
}

void Display::Zoom(QPoint &center, double magnitude)
{
    // New midpoint

    CartesianLoc move
    (
        &cs,
        double(center.x()) * (magnitude - 1.0) / magnitude, // X
        double(center.y()) * (magnitude - 1.0) / magnitude  // Y
    );

    move.TransformTo(WORLD);

    cs.center_x = move.x();
    cs.center_y = move.y();

    cs.x_res /= magnitude;
    cs.y_res /= magnitude;

    cs.RecalcBaseVectors();
}
