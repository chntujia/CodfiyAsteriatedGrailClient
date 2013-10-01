#ifndef SHOWAREA_H
#define SHOWAREA_H

#include <QGraphicsObject>
#include "data/Card.h"
class ShowArea : public QGraphicsObject
{
    Q_OBJECT
public:
    ShowArea();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void showCards(QList<Card*>);
private:
    QList<Card*> cards;
    
};

#endif // SHOWAREA_H
