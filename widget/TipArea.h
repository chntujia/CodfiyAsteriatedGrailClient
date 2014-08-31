#ifndef TIPAREA_H
#define TIPAREA_H

#include <QGraphicsObject>
#include <QComboBox>
#include <QGraphicsProxyWidget>
#include "widget/CardItem.h"
#include "data/SafeList.h"
class TipArea : public QGraphicsObject
{
    Q_OBJECT
public:
    TipArea();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setMsg(QString msg);
    void addBoxItem(QString text);
    void reset();
    void clearStatus();
    void showBox();
    void showStatus(int id);
    void adjustCards();
    int getBoxCurrentIndex(){return box->currentIndex();}
    QString getBoxCurrentText(){return box->currentText();}
    int getSelectedCardID(){return selectedCard->getID();}
    void win(int team);
    int getWidth() { return width; }
    int getHeight() { return height; }

public slots:
    void onCardSelected(int id);
    void onCardUnselected(int id);
private:
    int width;
    int height;
    bool winflag;
    QPixmap background;
    QPixmap winpix;
    QString msg;
    QComboBox *box;
    QGraphicsProxyWidget *boxItem;
    SafeList<CardItem*> cardItems;
    Card* selectedCard;    
};

#endif // TIPAREA_H
