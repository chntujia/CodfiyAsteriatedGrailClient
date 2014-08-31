#include "Timelinebar.h"

TimelineBar::TimelineBar(QGraphicsScene *scene, int width): timeline(NULL)
{
    progressBar = new QProgressBar;
    progressBar->setTextVisible(false);
    progressBar->setFixedWidth(width);

    progressBarItem = scene->addWidget(progressBar);
    progressBarItem->setVisible(false);
}

void TimelineBar::setPos(int x, int y)
{
    progressBarItem->setPos(x, y);
}

void TimelineBar::startCounting(int seconds)
{
    stopCounting();
    int tune = 10;
    progressBarItem->setVisible(true);
    progressBar->setRange(0, seconds * tune);

    timeline = new QTimeLine(seconds * 1000);
    timeline->setFrameRange(0, seconds * tune);
    QObject::connect(timeline, SIGNAL(frameChanged(int)), progressBar, SLOT(setValue(int)));
    QObject::connect(timeline, SIGNAL(finished()), progressBar, SLOT(hide()));
    timeline->start();
}

void TimelineBar::stopCounting()
{
    if(timeline){
        timeline->stop();
        delete timeline;
        timeline = NULL;
    }
    progressBar->setVisible(false);
}
