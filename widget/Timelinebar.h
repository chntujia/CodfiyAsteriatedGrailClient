#ifndef TIMELINEBAR_H
#define TIMELINEBAR_H

#include <QProgressBar>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QTimeLine>

class TimelineBar
{
public:
    TimelineBar(QGraphicsScene* scene, int width);
    void setPos(int x, int y);
    void startCounting(int seconds);
    void stopCounting();
private:
    QProgressBar *progressBar;
    QGraphicsProxyWidget *progressBarItem;
    QTimeLine *timeline;
};

#endif // TIMELINEBAR_H
