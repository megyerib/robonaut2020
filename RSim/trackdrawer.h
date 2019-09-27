#ifndef TRACKDRAWER_H
#define TRACKDRAWER_H

#include "widgetdrawer.h"
#include "simsetting.h"
#include <QObject>
#include <QImage>
#include <QTimer>
#include "navimath2d.h"

class TrackDrawer : public QObject, public WidgetDrawer
{
    Q_OBJECT

    SimSetting& setting;
    CartesianCS* WindowCs;
    QTimer refreshTimer;

    double trackWidth;
    double trackHeight;

public:
    TrackDrawer(QWidget& w, SimSetting& s);

    void Draw() override;
    void Drag(QPoint &start, QPoint &end) override;
    void Zoom(QPoint &center, double magnitude) override;

public slots:
    void Refresh();
};

#endif // TRACKDRAWER_H
