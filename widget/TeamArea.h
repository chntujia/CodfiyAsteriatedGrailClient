#ifndef TEAMAREA_H
#define TEAMAREA_H

#include <QGraphicsObject>
#include "data/Team.h"
class TeamArea : public QGraphicsObject
{
    Q_OBJECT
public:
    TeamArea();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void addRoundBy1(){round=round>=99?0:round+1;}
    void changeDroppedCardNum(int delta){droppedCardNum+=delta;}
    void changeLeftCardNum(int delta){leftCardNum+=delta;}
    void setDroppedCardNum(int howMany){droppedCardNum=howMany;}
    void setLeftCardNum(int howMany){leftCardNum=howMany;}
private:
    Team* red,*blue;
    QPixmap background;
    QPixmap Rgrail,Bgrail,Pcrystal,Pgem,Rdice[6],Bdice[6],Num[10];
    int width;
    int height;
    int avg;
    int round;
    int droppedCardNum,leftCardNum;
};

#endif // TEAMAREA_H
