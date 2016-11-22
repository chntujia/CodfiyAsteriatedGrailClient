#include <QPainter>
#include <widget/GUI.h>
#include "HandArea.h"
#include "data/DataInterface.h"
#include "logic/Logic.h"
#include "PlayerItem.h"

static QRectF HandAreaRect(0, 0, 750, 150);

HandArea::HandArea()
{
    least=most=1;
    selectedCards.clear();
}

QRectF HandArea::boundingRect() const
{
    return HandAreaRect;
}

void HandArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

void HandArea::addCardItem(Card* card)
{
    CardItem* cardItem=new CardItem(card);
    cardItem->setParentItem(this);
    connect(cardItem,SIGNAL(cardSelected(int)),this,SLOT(onCardSelected(int)));
    connect(cardItem,SIGNAL(cardUnselected(int)),this,SLOT(onCardUnselected(int)));
    cardItems << cardItem;

    adjustCards();
}
void HandArea::onCardSelected(int id)
{
    if(selectedCards.count()>=most)
    {
        int firstCardID=selectedCards.takeFirst()->getID();
        foreach(CardItem *ptr,cardItems)
        {
            if (ptr->getCard()->getID()==firstCardID)
            {
                ptr->reset();
                break;
            }
        }
    }
    selectedCards<<dataInterface->getCard(id);
    if(selectedCards.count()>=least)
        emit cardReady();
}

void HandArea::onCardUnselected(int id)
{
    selectedCards.removeOne(dataInterface->getCard(id));
    if(selectedCards.count()<least)
    {
        gui->getPlayerArea()->reset();
        gui->getDecisionArea()->disable(0);
        emit cardUnready();
    }
    else
        emit cardReady();
}

void HandArea::reset()
{
    disableAll();
    foreach(CardItem* ptr, cardItems)
    {
        ptr->reset();
    }
    selectedCards.clear();
}

void HandArea::enableAttack()
{
    foreach(CardItem *ptr,cardItems)
        if(ptr->getCard()->getType()=="attack")
        {
            ptr->setEnabled(1);
            ptr->setOpacity(1);
        }
}
void HandArea::disableAttack()
{
    foreach(CardItem *ptr,cardItems)
        if(ptr->getCard()->getType()=="attack")
        {
            ptr->setEnabled(0);
            ptr->setOpacity(0.8);
        }
}
void HandArea::enableMagic()
{
    foreach(CardItem *ptr,cardItems)
        if(ptr->getCard()->getType()=="magic")
        {
            ptr->setEnabled(1);
            ptr->setOpacity(1);
        }
}
void HandArea::disableMagic()
{
    foreach(CardItem *ptr,cardItems)
        if(ptr->getCard()->getType()=="magic")
        {
            ptr->setEnabled(0);
            ptr->setOpacity(0.8);
        }
}

int HandArea::getElementCount()
{
    QSet<QString> elements;
    foreach(CardItem *ptr,cardItems)
        elements.insert(ptr->getCard()->getElement());
    return elements.size();
}

void HandArea::disableElement(QString element)
{
    foreach(CardItem *ptr,cardItems)
        if(ptr->getCard()->getElement()==element)
        {
            ptr->setEnabled(0);
            ptr->setOpacity(0.8);
        }
}
void HandArea::disableElementExcept(QString element)
{
    foreach(CardItem *ptr,cardItems)
        if(ptr->getCard()->getElement()!=element)
        {
            ptr->setEnabled(0);
            ptr->setOpacity(0.8);
        }
}
void HandArea::enableElement(QString element)
{
    foreach(CardItem *ptr,cardItems)
        if(ptr->getCard()->getElement()==element)
        {
            ptr->setEnabled(1);
            ptr->setOpacity(1);
        }
}
void HandArea::enableTypeAndElement(QString type, QString element)
{
    foreach(CardItem *ptr,cardItems)
        if(ptr->getCard()->getElement()==element && ptr->getCard()->getType()==type)
        {
            ptr->setEnabled(1);
            ptr->setOpacity(1);
        }
}

void HandArea::enableProperty(QString property)
{
    foreach(CardItem *ptr,cardItems)
        if(ptr->getCard()->getProperty()==property)
        {
            ptr->setEnabled(1);
            ptr->setOpacity(1);
        }
}

void HandArea::enableMoDan()
{
    foreach(CardItem *ptr,cardItems)
        if(ptr->getCard()->getName()==QStringLiteral("魔弹"))
        {
            ptr->setEnabled(1);
            ptr->setOpacity(1);
        }
}

void HandArea::enableSpecility(QString specility)
{
    CardItem *ptr;
    QStringList specialityList;
    foreach(ptr,cardItems)
    {
        specialityList=ptr->getCard()->getSpecialityList();
        if(specialityList.contains(specility))
        {
            ptr->setEnabled(1);
            ptr->setOpacity(1);
        }
    }
}
bool HandArea::checkSpecility(QString specility)
{
    CardItem *ptr;
    QStringList specialityList;
    foreach(ptr,cardItems)
    {
        specialityList=ptr->getCard()->getSpecialityList();
        if(specialityList.contains(specility))
            return 1;
    }
    return 0;
}

bool HandArea::checkElement(QString element)
{
    foreach(CardItem *ptr,cardItems)
        if(ptr->getCard()->getElement()==element)
            return 1;
    return 0;
}

bool HandArea::checkType(QString type)
{
    foreach(CardItem *ptr,cardItems)
        if(ptr->getCard()->getType()==type)
            return 1;
    return 0;
}

void HandArea::enableAll()
{
    CardItem *ptr;
    foreach(ptr,cardItems)
    {
        ptr->setEnabled(1);
        ptr->setOpacity(1);
    }
}
void HandArea::disableAll()
{
    CardItem *ptr;
    foreach(ptr,cardItems)
    {
        ptr->setEnabled(0);
        ptr->setOpacity(0.8);
    }
}

void HandArea::removeCardItem(Card* card)
{
    int i=0;
    foreach(CardItem *ptr,cardItems)
    {
        if(ptr->getCard()==card)
        {
            cardItems.removeAt(i);
            delete ptr;
        }
        i++;
    }
    selectedCards.removeOne(card);
    adjustCards();
}

void HandArea::cleanCardItem()
{
    foreach(CardItem *ptr,cardItems)
    {
        delete ptr;
    }
    cardItems.clear();
    selectedCards.clear();
}

void HandArea::adjustCards()
{
    if(cardItems.isEmpty())
        return;
    int n = cardItems.size();
    int card_skip;
    if(n > 6)
        card_skip = 600/(n-1);
    else
        card_skip = 100;

    int i;
    for(i=0; i<n; i++)
    {
        cardItems[i]->setZValue(0.1 * i);
        cardItems[i]->setPos(QPointF(i*card_skip, 0));
    }
}

SafeList<Card*> HandArea::getSelectedCards()
{
    return selectedCards;
}
