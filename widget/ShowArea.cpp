#include "ShowArea.h"
#include "Animation.h"
#include <QPainter>
//#include <windows.h>

static QRectF ShowAreaRect(0, 0, 500, 150);
ShowArea::ShowArea()
{

}
QRectF ShowArea::boundingRect() const
{
    return ShowAreaRect;
}

void ShowArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int n = cards.size();
    int i,j;
    int card_skip;
    int offset=0;
    if(n > 5)
        card_skip = 400/(n-1);
    else
    {
        card_skip = 100;
        offset=250-n*0.5*100;
    }
    Card*card;
    QPixmap pixmap;
    int width;
    int height;
    for(i=0;i<n;i++)
    {
        card=cards[i];
        pixmap=QPixmap(card->getSource());
        width=pixmap.width();
        height=pixmap.height();
        painter->drawPixmap(offset+card_skip*i,0,pixmap);
        painter->drawText(offset+width*0.8+i*card_skip,height*0.68,card->getProperty());
        for(j=0;j<card->getHasSpeciality();j++)
            painter->drawText(offset+width*0.15+i*card_skip,height*(0.76+j*0.13),card->getSpecialityList().at(j));

    }

}

void ShowArea::showCards(QList<Card *> cards)
{
    this->cards.clear();
    foreach(Card* ptr,cards)
        this->cards<<ptr;
    update();

    int howMany = cards.length();
    int card_skip;
    int offset = 0;
    if(howMany > 5)
        card_skip = 450/(howMany-1);
    else
    {
        card_skip = 100;
        offset=250-howMany*0.5*100;
    }
    for(int i = 0;i <howMany;i++)
    {
        CardItem* cardItem;
        cardItem = new CardItem(cards[i]);
        animation->itemFlash(cardItem,x() + offset+card_skip*i + cardItem->boundingRect().width()/2,y()+cardItem->boundingRect().height()/2)->start(QAbstractAnimation::DeleteWhenStopped);
    }

}


