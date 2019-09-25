#ifndef WIDGETDRAWER_H
#define WIDGETDRAWER_H

#include <QWidget>
#include <QPoint>

class WidgetDrawer
{
protected:
    QWidget& widget;
    WidgetDrawer(QWidget& w);

public:

    virtual ~WidgetDrawer() = default;

    virtual void Draw() = 0;
    virtual void Zoom(QPoint& center, double magnitude) = 0;
    virtual void Drag(QPoint& start, QPoint& end) = 0;
};

#endif // WIDGETDRAWER_H
