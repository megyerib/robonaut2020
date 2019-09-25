#ifndef TRACKDRAWER_H
#define TRACKDRAWER_H

#include "widgetdrawer.h"
#include "simsetting.h"
#include <QImage>
#include "navimath2d.h"

class TrackDrawer : public WidgetDrawer
{
    SimSetting& setting;
    CartesianCS* WindowCs;

    double trackWidth;
    double trackHeight;

public:
    TrackDrawer(QWidget& w, SimSetting& s);

    void Draw() override;
    void Drag(QPoint &start, QPoint &end) override;
    void Zoom(QPoint &center, double magnitude) override;
};

#endif // TRACKDRAWER_H
