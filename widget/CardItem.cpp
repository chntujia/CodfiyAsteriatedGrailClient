#include "CardItem.h"
#include <QPainter>

CardItem::CardItem(Card* card)
{
    this->card=card;
    pixmap=QPixmap(card->getSource());

    this->width=pixmap.width();
    this->height=pixmap.height();
    this->selected=0;
    setToolTip(card->getDescription().replace("#","\n"));
}
CardItem::~CardItem()
{
}


QRectF CardItem::boundingRect() const
{
    return QRectF(0, 0, width, height);
}
void CardItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0, 0, pixmap);
    painter->drawText(width*0.8,height*0.68,this->card->getProperty());
    int i;
    for(i=0;i<this->card->getHasSpeciality();i++)
        painter->drawText(width*0.15,height*(0.76+i*0.13),this->card->getSpecialityList().at(i));
}
Card* CardItem::getCard()
{
    return this->card;
}
bool CardItem::isSelected()
{
    return this->selected;
}
void CardItem::setSelected(bool status)
{
    this->selected=status;
}

void CardItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!this->selected)
    {
        selected=1;
        setY(y()-20);
        emit cardSelected(card->getID());
    }
    else
    {
        selected=0;
        setY(y()+20);
        emit cardUnselected(card->getID());
    }
}

