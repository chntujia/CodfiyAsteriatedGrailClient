#include "PlayerArea.h"
#include "data/DataInterface.h"
#include "widget/GUI.h"
#include "data/Player.h"
#include <QPainter>
static QRectF PlayerAreaRect(0, 0, 1000, 700);
static const QPointF PlayerPos6[]={QPointF(5,510),QPointF(698,270),QPointF(550,60),QPointF(350,60),QPointF(145,60),QPointF(5,270)};
static const QPointF PlayerPos4[]={QPointF(5,510),QPointF(698,270),QPointF(350,60),QPointF(5,270)};
PlayerArea::PlayerArea():least(1),most(1)
{

}

void PlayerArea::init(bool playing)
{
    int i;
    //根据座次建立
    QList<Player*> playerList = dataInterface->getPlayerList();
    int playerSum = playerList.count();
    clear();
    for(i=0;i<playerSum;i++)
    {
        playerItems<<new PlayerItem(playerList[i]);
        connect(playerItems[i],SIGNAL(playerSelected(int)),this,SLOT(onPlayerSelected(int)));
        connect(playerItems[i],SIGNAL(playerUnselected(int)),this,SLOT(onPlayerUnselected(int)));
        playerItems[i]->setParentItem(this);
        if(playerSum==4)
            playerItems[i]->setPos(PlayerPos4[i]);
        else
            playerItems[i]->setPos(PlayerPos6[i]);
        playerList[i]->setPos(i);
    }
    //根据ID排序
    sortPlayerItems();
    dataInterface->sortPlayers();
    int myID=dataInterface->getID();
    //观战的人跳过
    if(myID != GUEST && playing){
        //设置队友        
        playerList=dataInterface->getPlayerList();
        Player* player=playerList[myID];
        int myColor=player->getColor();
        for(i=0;i<playerList.count();i++)
            if(playerList[i]->getColor()!=myColor)
                enemy<<i;
    }
    currentPlayerID=dataInterface->getFirstPlayerID();
}

void PlayerArea::clear()
{
    foreach(PlayerItem*p, playerItems)
    {
        delete p;
    }
    playerItems.clear();
    enemy.clear();
}

void PlayerArea::reset()
{
    disableAll();
    foreach(PlayerItem* ptr,playerItems)
        ptr->setSelected(0);
    selectedPlayers.clear();
}

QRectF PlayerArea::boundingRect() const
{
    return PlayerAreaRect;
}

void PlayerArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(!playerItems.empty()){
        PlayerItem* currentPlayerItem;
        currentPlayerItem=playerItems[currentPlayerID];
        painter->drawPixmap(currentPlayerItem->x()-5,currentPlayerItem->y()-4,QPixmap("resource/yourTurn.png"));
    }
}

void PlayerArea::setCurrentPlayerID(int id)
{
    PictureContainer* picture;
    PlayerItem* currentPlayerItem;
    currentPlayerID=id;
    currentPlayerItem=playerItems[currentPlayerID];

    picture = new PictureContainer();
    picture->setPixmap(currentPlayerItem->getPlayer()->getFaceSource());
    animation->itemFlash(picture,currentPlayerItem->x()+picture->boundingRect().width()/2,currentPlayerItem->y()+picture->boundingRect().height()/2)->start(QAbstractAnimation::DeleteWhenStopped);
}

void PlayerArea::drawLineBetween(int id1, int id2)
{
    if(id1<0 || id1>playerItems.size())
        return;
    if(id2<0 || id2>playerItems.size())
        return;
    if(id1==id2)
        return;
    int id = dataInterface->getMyself()->getID();
    int x1,y1,x2,y2;
    x1=playerItems.at(id1)->x()+playerItems.at(id1)->boundingRect().width()/2;
    y1=playerItems.at(id1)->y()+playerItems.at(id1)->boundingRect().height()/2;
    if(id==id1){
        x1=450;
        y1=playerItems.at(id)->y()+5;
    }
    x2=playerItems.at(id2)->x()+playerItems.at(id2)->boundingRect().width()/2;
    y2=playerItems.at(id2)->y()+playerItems.at(id2)->boundingRect().height()/2;
    if(id==id2){
        x2=450;
        y2=playerItems.at(id)->y()+5;
    }
    animation->drawLineBetween(x1,y1,x2,y2);
}

void PlayerArea::sortPlayerItems()
{
    int i,j;
    int max=dataInterface->getPlayerMax();
    for(i=0;i<max;i++)
        for(j=0;j<max;j++)
        {
            if(playerItems[j]->getPlayer()->getID()==i)
            {
                playerItems.swap(i,j);
                break;
            }
        }
}
void PlayerArea::enablePlayerItem(int id)
{
    playerItems[id]->setEnabled(1);
    playerItems[id]->setOpacity(1);
}

void PlayerArea::disablePlayerItem(int id)
{
    playerItems[id]->setEnabled(0);
    playerItems[id]->setOpacity(0.6);
}

void PlayerArea::enableAll()
{
    foreach(PlayerItem*ptr,playerItems)
    {
        ptr->setEnabled(1);
        ptr->setOpacity(1);
    }
}

void PlayerArea::disableAll()
{
    foreach(PlayerItem*ptr,playerItems)
    {
        ptr->setEnabled(0);
        ptr->setOpacity(0.6);
    }
}

void PlayerArea::enableMate(bool includeYou)
{
    int i;
    for(i=0;i<playerItems.count();i++)
        if(!enemy.contains(i))
            enablePlayerItem(i);
    if(!includeYou)
        disablePlayerItem(dataInterface->getID());
}
void PlayerArea::enableEnemy()
{
    int i;
    for(i=0;i<playerItems.count();i++)
        if(enemy.contains(i))
            enablePlayerItem(i);
}

void PlayerArea::onPlayerSelected(int id)
{
    if(selectedPlayers.count()>=most)
    {
        int firstPlayerID=selectedPlayers.takeFirst()->getID();
        playerItems[firstPlayerID]->setSelected(0);
    }
    selectedPlayers<<playerItems[id]->getPlayer();
    if(selectedPlayers.count()<least)
        return;
    emit playerReady();
}

void PlayerArea::onPlayerUnselected(int id)
{
    selectedPlayers.removeOne(playerItems[id]->getPlayer());
    if(selectedPlayers.count()<least)
    {
        gui->getDecisionArea()->disable(0);
        emit playerUnready();
    }
}
QList<Player*> PlayerArea::getSelectedPlayers()
{
    return selectedPlayers;
}
