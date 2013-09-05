#ifndef ANIMATION_H
#define ANIMATION_H
#include "RoomView.h"
#include "CardItem.h"
#include <QPixmap>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsObject>

#include <QDebug>
class PictureContainer:public QGraphicsObject
{
    Q_OBJECT
public:
    PictureContainer();
    void setPixmap(QString picSrc){pixmap.load(picSrc);width = pixmap.width();height = pixmap.height();}
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    QPixmap pixmap;
    int width;
    int height;
};
class LineContainer: public QGraphicsObject
{
    Q_OBJECT
    friend class Animation;
public:
    LineContainer(int x1,int y1,int x2,int y2);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setRate(int x,int y){xRate=x;yRate=y;}
public slots:
    void grow(){x2+=xRate*1.2;y2+=yRate*1.2;update();}
private:
    int x1,x2,y1,y2;
    int xRate,yRate;
};

class Animation:QObject
{
    Q_OBJECT
public:
    Animation();
    void setRoomScene(RoomScene *view);

    //如果希望产生动画效果的item是临时生成，动画之后又自动消失的，一定要把new出的临时对象装进这个列表，否则会内存泄露
    //这个函数内有线程锁，会等待上一次使用临时对象的动画（目前只有itemFlash）完毕之后，销毁上一次的临时对象，然后返回容纳临时对象的容器
    QList<QGraphicsObject*> getTempItems();

    //对QGraphicsObject对象产生闪烁效果
    //QGraphicsObject对象一定要装在getTempItems（）返回的QList中！
    //x和y是希望闪烁的对象的中心点位置，注意不是左上角位置！
    QParallelAnimationGroup* itemFlash(QGraphicsObject* item,int x,int y);
    QParallelAnimationGroup* itemFade(QGraphicsObject*  item, int x, int y);
    void drawLineBetween(int x1, int y1, int x2, int y2);

public slots:
    //itemFlash结束之后解锁，允许下一个闪烁效果获取临时对象列表
    void unlock();
private:
    RoomScene *view;
    bool busy;
    QList<QGraphicsObject*> tempItems;
};

extern Animation *animation;
#endif // ANIMATION_H
