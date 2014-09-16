#ifndef CARDITEM_H
#define CARDITEM_H

#include <QGraphicsObject>
#include <QPixmap>
#include "data/Card.h"
class CardItem : public QGraphicsObject
{
    Q_OBJECT
public:
    CardItem(int id);
    CardItem(Card* card);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    Card* getCard();
    bool isSelected();
    void reset();

private:
    Card *card;
    int width;
    int height;
    bool selected;
    QPixmap pixmap;

signals:
    void cardSelected(int id);
    void cardUnselected(int id);
    
};

#endif // CARDITEM_H
