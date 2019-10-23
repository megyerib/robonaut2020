#pragma once

#include <QObject>
#include <QTimer>

class Refreshable : public QObject
{
    Q_OBJECT

    QTimer*  refreshTimer;

protected:
    ~Refreshable() = default;
    void InitRefreshTimer(double interval /* [s] */);

private slots:
    virtual void Refresh() = 0;
};
