#ifndef TRACKDRAWER_H
#define TRACKDRAWER_H

#include "simsetting.h"
#include <QObject>
#include <QImage>
#include <QTimer>
#include <QWidget>
#include "navimath2d.h"

class TrackDrawer : public QObject
{
    Q_OBJECT

    QWidget& widget;

    SimSetting& setting;
    CartesianCS* WindowCs;
    CartesianCS* BgCs;
    QTimer refreshTimer;

    double trackWidth;
    double trackHeight;

public:
    TrackDrawer(QWidget& w, SimSetting& s);

    void Draw();
    void Drag(QPoint &start, QPoint &end);
    void Zoom(QPoint &center, double magnitude);

public slots:
    void Refresh();
};

#endif // TRACKDRAWER_H
