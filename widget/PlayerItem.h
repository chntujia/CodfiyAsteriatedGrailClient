#ifndef PLAYERITEM_H
#define PLAYERITEM_H

#include <QGraphicsObject>
#include "data/Player.h"
#include <QPushButton>
class StatusItem : public QGraphicsObject
{
    friend class PlayerItem;
public:
    StatusItem(BasicStatus*status);
    StatusItem(Token *token);
    StatusItem(int specialStatusID);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    BasicStatus*status;
    Token*token;
    int specialStatusID;
    QPixmap Num[10];
    QPixmap pic;
    int type;
};

class PlayerItem : public QGraphicsObject
{
    Q_OBJECT
public:
    PlayerItem(Player* player);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    Player *getPlayer();
    void setSelected(bool yes);
    bool isSelected();
    void adjustStatusItems();
    void setReady(bool r) { ready = r; }
    void setLeader(int l){withleader = l;}
public slots:
    void addBasicStatusItem(BasicStatus* status);
    void removeBasicStatusItem(BasicStatus *status);
    void addSpecialStatusItem(int specialStatusID);
    void removeSpecialStatusItem(int specialStatusID);
    void addTokenItem(Token*);
signals:
    void playerSelected(int);
    void playerUnselected(int);
private:
    Player* player;
    QPixmap frame;
    QPixmap face;
    QPixmap gem;
    QPixmap crystal;
    QPixmap card;
    QPixmap overflow;
    QPixmap heal;
    QPixmap cardRemain;
    QPixmap healRemain;
    QPixmap energyRemain;
    QPixmap redReadyFrame;
    QPixmap blueReadyFrame;
    QPixmap readyStatus;
    QPixmap readyWithLeader;
    int width;
    int height;
    bool selected;
    bool ready;
    int withleader;
    SafeList<StatusItem*> statusItems;

};

#endif // PLAYERITEM_H
