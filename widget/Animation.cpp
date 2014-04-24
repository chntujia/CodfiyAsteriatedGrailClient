#include "Animation.h"
#include "data/DataInterface.h"
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QCoreApplication>
#include <QThread>
#include <QList>
#include <QTimeLine>
Animation *animation = NULL;

PictureContainer::PictureContainer():QGraphicsObject()
{
    width = 0;
    height = 0;
}

QRectF PictureContainer::boundingRect() const
{
    return QRectF(0,0,width,height);
}

void PictureContainer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0, 0, pixmap);
}

LineContainer::LineContainer(int x1, int y1, int x2, int y2):x1(x1),y1(y1),x2(x2),y2(y2)
{
}

QRectF LineContainer::boundingRect() const
{
    return QRectF(0,0,1000,1000);
}
void LineContainer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush brush(QColor(255, 215, 0));
    QPen pen(brush,5);
    painter->setPen(pen);
    painter->drawLine(QPointF(x1,y1),QPointF(x2,y2));
}

Animation::Animation():QObject()
{   
    busy = false;
}
//暂时不再使用!!
//如果希望产生动画效果的item是临时生成，动画之后又自动消失的，一定要把new出的临时对象装进这个列表，否则会内存泄露
//这个函数内有线程锁，会等待上一次使用临时对象的动画（目前只有itemFlash）完毕之后，销毁上一次的临时对象，然后返回容纳临时对象的容器
SafeList<QGraphicsObject *> Animation::getTempItems()
{
//    while(busy)
//    {
//        QCoreApplication::processEvents(QEventLoop::AllEvents,1000);
//    }
    return tempItems;
}
//暂时不再使用!!
//itemFlash结束之后解锁，允许下一个闪烁效果获取临时对象列表
void Animation::unlock()
{
    QGraphicsObject* ptr;
    foreach(ptr,tempItems)
    {
        delete ptr;
    }
    tempItems.clear();
    busy = false;
}

void Animation::setRoomScene(RoomScene *view)
{
    this->view = view;
}

//对QGraphicsObject对象产生闪烁效果
//QGraphicsObject对象一定要装在getTempItems（）返回的QList中！
//x和y是希望闪烁的对象的中心点位置，注意不是左上角位置！
QParallelAnimationGroup* Animation::itemFlash(QGraphicsObject*  item, int x, int y)
{

    int w = item->boundingRect().width();
    int h = item->boundingRect().height();
    this->view->addItem(item);

    QPropertyAnimation *posAni = new QPropertyAnimation(view);
    QPropertyAnimation *scaleAni = new QPropertyAnimation(view);
    QPropertyAnimation *opacityAni = new QPropertyAnimation(view);

    posAni->setTargetObject(item);
    posAni->setPropertyName("pos");
    posAni->setDuration(1000);
    posAni->setStartValue(QPoint(x - w/2,y-h/2));

    scaleAni->setTargetObject(item);
    scaleAni->setPropertyName("scale");
    scaleAni->setDuration(1000);
    scaleAni->setStartValue(1);

    opacityAni->setTargetObject(item);
    opacityAni->setPropertyName("opacity");
    opacityAni->setDuration(1000);
    opacityAni->setStartValue(0.7);

    w *= 3;
    h *= 3;
    posAni->setEndValue(QPoint(x - w/2,y-h/2));
    scaleAni->setEndValue(3);
    opacityAni->setEndValue(0);

    QParallelAnimationGroup *paraAniGroup = new QParallelAnimationGroup();
    paraAniGroup->addAnimation(posAni);
    paraAniGroup->addAnimation(scaleAni);
    paraAniGroup->addAnimation(opacityAni);
    connect(paraAniGroup,SIGNAL(finished()),item,SLOT(deleteLater()));
    busy = true;
    return paraAniGroup;

}

void Animation::drawLineBetween(int x1, int y1, int x2, int y2)
{
    LineContainer *line = new LineContainer(x1,y1,x1,y1);
    view->addItem(line);
    QTimeLine *timeLine=new QTimeLine(1000);
    timeLine->setFrameRange(0, 100);
    int howMany = 1000/timeLine->updateInterval();
    line->setRate((x2-x1)/howMany,(y2-y1)/howMany);
    connect(timeLine,SIGNAL(frameChanged(int)),line,SLOT(grow()));
    connect(timeLine,SIGNAL(finished()),line,SLOT(deleteLater()));
    connect(timeLine,SIGNAL(finished()),timeLine,SLOT(deleteLater()));
    timeLine->start();

}
