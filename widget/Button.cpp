#include "Button.h"
#include <QPainter>
#include "DataInterface.h"

Button::Button(int id,QString msg)
{
    background=QPixmap("resource/button1.png");
    height=background.height();
    width=background.width();
    selected=0;
    this->id=id;
    this->msg=msg;
    setToolTip(dataInterface->getButtonInfo(msg));
    setEnabled(0);
    setOpacity(0.8);
}
QRectF Button::boundingRect() const
{
    return QRectF(0, 0, width, height);
}
void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{       
    if(isEnabled())
        painter->drawPixmap(0, 0, QPixmap("resource/Button6.png"));
    else
        painter->drawPixmap(0, 0, QPixmap("resource/Button1.png"));

    QFont font;
    font.setBold(1);
    painter->setFont(font);
    painter->drawText(QRectF(0, 0, width, height),Qt::AlignCenter,msg);

    if(selected)
        painter->drawPixmap(0,0,QPixmap("resource/buttonSelected.png"));
}
void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!this->selected)
    {
        selected=1;
        update();
        emit buttonSelected(id);
    }
    else
    {
        selected=0;
        update();
        emit buttonUnselected(id);
    }
}
