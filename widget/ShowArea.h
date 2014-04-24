#ifndef SHOWAREA_H
#define SHOWAREA_H

#include <QGraphicsObject>
#include "data/Card.h"
#include "data/SafeList.h"
class ShowArea : public QGraphicsObject
{
    Q_OBJECT
public:
    ShowArea();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void showCards(SafeList<Card*>);
private:
    SafeList<Card*> cards;
    
};

#endif // SHOWAREA_H
