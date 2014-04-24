#ifndef PLAYERAREA_H
#define PLAYERAREA_H

#include <QGraphicsObject>
#include "PlayerItem.h"
#include "data/SafeList.h"
class LineContainer;
class PlayerArea : public QGraphicsObject
{
    Q_OBJECT
    friend class GUI;
public:
    PlayerArea();
    void init(bool playing = true);
    void clear();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void sortPlayerItems();
    void enablePlayerItem(int id);
    void enableAll();
    void disablePlayerItem(int id);
    void enableEnemy();
    void enableMate(bool includeYou=false);
    void disableAll();
    void reset();
    void setCurrentPlayerID(int id);
    void drawLineBetween(int id1,int id2);
    void setQuota(int howMany){least=most=howMany;}
    void setQuota(int least,int most){this->least=least;this->most=most;}
    SafeList<Player *> getSelectedPlayers();
    PlayerItem* getPlayerItem(int id){return playerItems.at(id);}

public slots:
    void onPlayerSelected(int id);
    void onPlayerUnselected(int id);
signals:
    void playerReady();
    void playerUnready();
private:
    SafeList<PlayerItem*> playerItems;
    SafeList<Player*> selectedPlayers;
    int least,most;
    int currentPlayerID;
    SafeList<int> enemy;
};

#endif // PLAYERAREA_H
