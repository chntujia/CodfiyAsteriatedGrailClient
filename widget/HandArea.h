#ifndef HANDAREA_H
#define HANDAREA_H

#include <QGraphicsObject>
#include "CardItem.h"
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
    void enableMagic();
    void disableMagic();
    void disableElement(QString element);
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
    QList<Card*> getSelectedCards();
    void addCardItem(Card* card);
    void removeCardItem(Card* card);
    void cleanCardItem();
    void setQuota(int howMany){least=most=howMany;}
    void setQuota(int least,int most){this->least=least;this->most=most;}
    QList<CardItem*> getHandCardItems(){return cardItems;}
signals:
    void cardReady();
    void cardUnready();
public slots:

    void onCardSelected(int id);
    void onCardUnselected(int id);

private:
    QPixmap pixmap;
    QList<CardItem*> cardItems;
    QList<Card*> selectedCards;
    int least;
    int most;
};


#endif // HANDAREA_H
