#ifndef HANDAREA_H
#define HANDAREA_H

#include <QGraphicsObject>
#include "CardItem.h"
#include "data/SafeList.h"
class HandArea : public QGraphicsObject
{
    Q_OBJECT
    friend class GUI;
public:
    HandArea();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void adjustCards();
    void enableAttack();
    void disableAttack();
    void enableMagic();
    void disableMagic();
    int getElementCount();
    void disableElement(QString element);
    void disableElementExcept(QString element);
    void enableElement(QString element);
    void enableMoDan();
    void enableAll();
    void disableAll();
    void enableProperty(QString property);
    void enableSpecility(QString specility);
    bool checkSpecility(QString specility);
    bool checkElement(QString element);
    bool checkType(QString type);
    void reset();
    SafeList<Card *> getSelectedCards();
    void addCardItem(Card* card);
    void removeCardItem(Card* card);
    void cleanCardItem();
    void setQuota(int howMany){least=most=howMany;}
    void setQuota(int least,int most){this->least=least;this->most=most;}
    SafeList<CardItem*> getHandCardItems(){return cardItems;}
    void enableTypeAndElement(QString type, QString element);
signals:
    void cardReady();
    void cardUnready();
public slots:

    void onCardSelected(int id);
    void onCardUnselected(int id);

private:
    QPixmap pixmap;
    SafeList<CardItem*> cardItems;
    SafeList<Card*> selectedCards;
    int least;
    int most;
};


#endif // HANDAREA_H
