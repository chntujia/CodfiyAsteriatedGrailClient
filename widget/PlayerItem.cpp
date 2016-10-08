#include "PlayerItem.h"
#include "data/DataInterface.h"
#include "logic/Logic.h"
#include <QPainter>
#include <QPushButton.h>

StatusItem::StatusItem(BasicStatus* status)
{
    this->status=status;
    type=0;
    Card*card=status->card;
    QString tip;
    tip=card->getInfo();
    for(int i=0;i<card->getHasSpeciality();i++)
        tip+='\n'+card->getSpecialityList().at(i);
    setToolTip(tip);
    switch(status->type)
    {
    case 0:
        pic=QPixmap("resource/status/du.png");
        break;
    case 1:
        pic=QPixmap("resource/status/xu.png");
        break;
    case 2:
        pic=QPixmap("resource/status/dun.png");
        break;
    case 31:
        pic=QPixmap("resource/status/fengFeng.png");
        break;
    case 32:
        pic=QPixmap("resource/status/shuiFeng.png");
        break;
    case 33:
        pic=QPixmap("resource/status/huoFeng.png");
        break;
    case 34:
        pic=QPixmap("resource/status/diFeng.png");
        break;
    case 35:
        pic=QPixmap("resource/status/leiFeng.png");
        break;
    case 4:
        pic=QPixmap("resource/status/wei.png");
        break;
    case 5:
        pic=QPixmap("resource/status/xun.png");
        break;
    }
}

StatusItem::StatusItem(int specialStatusID)
{
    type=1;
    this->specialStatusID=specialStatusID;
    switch(specialStatusID)
    {
    case 0:
        pic=QPixmap("resource/status/ShuFu.png");
        setToolTip(QStringLiteral("五系束缚"));
        break;
    case 1:
        pic=QPixmap("resource/status/TiaoXin.png");
        setToolTip(QStringLiteral("挑衅"));
        break;
    case 2:
        pic=QPixmap("resource/status/LianJie.png");
        setToolTip(QStringLiteral("灵魂链接"));
        break;
    case 3:
        pic=QPixmap("resource/status/TongSheng.png");
        setToolTip(QStringLiteral("同生共死"));
        break;
    case 4:
        pic=QPixmap("resource/status/YueZhang.png");
        setToolTip(QStringLiteral("永恒乐章"));
        break;
    }
}

StatusItem::StatusItem(Token* token)
{
    type=2;
    this->token=token;
    switch(token->type)
    {
    case 0:
        pic=QPixmap("resource/status/token0.png");
        break;
    case 1:
        pic=QPixmap("resource/status/token1.png");
        break;
    case 2:
        pic=QPixmap("resource/status/token2.png");
        break;
    }
    setToolTip(token->tokenName+QStringLiteral("上限为")+QString::number(token->max)+")");
    Num[0]=QPixmap("resource/SmallNumber/Small0.png");
    Num[1]=QPixmap("resource/SmallNumber/Small1.png");
    Num[2]=QPixmap("resource/SmallNumber/Small2.png");
    Num[3]=QPixmap("resource/SmallNumber/Small3.png");
    Num[4]=QPixmap("resource/SmallNumber/Small4.png");
    Num[5]=QPixmap("resource/SmallNumber/Small5.png");
    Num[6]=QPixmap("resource/SmallNumber/Small6.png");
    Num[7]=QPixmap("resource/SmallNumber/Small7.png");
    Num[8]=QPixmap("resource/SmallNumber/Small8.png");
    Num[9]=QPixmap("resource/SmallNumber/Small9.png");
}

QRectF StatusItem::boundingRect() const
{
    return QRectF(0, 0, 15, 15);
}

void StatusItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
     painter->drawPixmap(0, 0, pic);
     if(type!=2)
         return;
     int howMany = token->num;
     if(howMany < 10 && howMany >= 0){
         painter->drawPixmap(0, 0, Num[howMany]);
     }
     else if(howMany > 9 && howMany < 100){
         int uints = howMany % 10;
         int tens = howMany / 10;
         painter->drawPixmap(0, 0, Num[tens]);
         painter->drawPixmap(7, 0, Num[uints]);
     }
}

PlayerItem::PlayerItem(Player* player):selected(0), ready(false)
{
    this->player=player;
    connect(player,SIGNAL(addBasicStatusSIG(BasicStatus*)),this,SLOT(addBasicStatusItem(BasicStatus*)));
    connect(player,SIGNAL(removeBasicStatusSIG(BasicStatus*)),this,SLOT(removeBasicStatusItem(BasicStatus*)));
    connect(player,SIGNAL(addTokenSIG(Token*)),this,SLOT(addTokenItem(Token*)));
    connect(player,SIGNAL(addSpecialStatusSIG(int)),this,SLOT(addSpecialStatusItem(int)));
    connect(player,SIGNAL(removeSpecialStatusSIG(int)),this,SLOT(removeSpecialStatusItem(int)));
    if(player->getColor() == 1)
        frame = QPixmap("resource/playerFrameRed.png");
    else if(player->getColor() == 0)
        frame = QPixmap("resource/playerFrameBlue.png");
    else
        frame = QPixmap("resource/playerFrameNone.png");
    redReadyFrame = QPixmap("resource/playerReadyRed.png");
    blueReadyFrame = QPixmap("resource/playerReadyBlue.png");
    readyStatus = QPixmap("resource/playerReady.png");
    readyWithLeader = QPixmap("resource/playerLeader.png");
    this->width=frame.width();
    this->height=frame.height();
    gem=QPixmap("resource/Egem.png");
    crystal=QPixmap("resource/Ecrystal.png");
    energyRemain=QPixmap("resource/energyRemain.png");
    card=QPixmap("resource/card.png");
    overflow=QPixmap("resource/overflow.png");
    heal=QPixmap("resource/heal.png");
    cardRemain=QPixmap("resource/cardRemain.png");
    healRemain=QPixmap("resource/healRemain.png");
}
QRectF PlayerItem::boundingRect() const
{
    return QRectF(-10, -10, width+20, height+20);
}
void PlayerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0, 0, frame);
    if(!player->getRoleID()){
    if(player->getColor() == 1)
        painter->drawPixmap(0, 0, redReadyFrame);
    else if(player->getColor() == 0)
        painter->drawPixmap(0, 0, blueReadyFrame);
    }
    painter->drawPixmap(2,6,player->getFaceSource());

    QFont font;
    font.setBold(1);
    painter->setFont(font);

    painter->drawText(0.3*width,0.11*height,QString::number(player->getID())+" - "+player->getNickname());

    int num=player->getHandCardNum();
    int max=player->getHandCardMax();
    QString entry=QString::number(num)+'/'+QString::number(max);
    painter->drawText(width*0.42,height*0.24,entry);
    int i,offset;
    for(i=0;num<=max && i<max;i++)
        if(i<num)
            painter->drawPixmap(width*0.62+card.width()*i,height*0.16, card);
        else
            painter->drawPixmap(width*0.62+card.width()*i,height*0.16, cardRemain);
    for(i=0;num>max && i<max;i++)
        painter->drawPixmap(width*0.62+card.width()*i,height*0.16, overflow);

    num=player->getCrossNum();
    max=player->getCrossMax();
    entry=QString::number(num)+'/'+QString::number(max);
    painter->drawText(width*0.42,height*0.37,entry);
    for(i=0;i<max;i++)
        if(i<num)
            painter->drawPixmap(width*0.62+card.width()*i,height*0.27, heal);
        else
            painter->drawPixmap(width*0.62+card.width()*i,height*0.27, healRemain);
    for(;i<num-max;i++)
        painter->drawPixmap(width*0.62+card.width()*i,height*0.27, heal);

    if(player->getEnergyMax()==4)
    {
        offset=width*0.74-gem.width()+1;
        painter->drawPixmap(offset,height*0.37,energyRemain);
    }
    else
        offset=width*0.74;
    for(i=0;i<player->getGem();i++)
        painter->drawPixmap(offset+i*(gem.width()-1),height*0.37,gem);

    offset+=i*(gem.width()-1);
    for(i=0;i<player->getCrystal();i++)
        painter->drawPixmap(i*(crystal.width()-1)+offset,height*0.37,crystal);

    if(player->getTap())
        painter->drawPixmap(0.265*width,0.4*height,QPixmap(player->getTapSource()));
    if(selected)
        painter->drawPixmap(-5,-4,QPixmap("resource/playerSelected.png"));
    if(ready)
        painter->drawPixmap(0, 0, readyStatus);
    if(withleader==1)
        painter->drawPixmap(0, 0, readyWithLeader);

}

void PlayerItem::addBasicStatusItem(BasicStatus *status)
{
    StatusItem *statusItem=new StatusItem(status);
    statusItem->setParentItem(this);
    statusItems<<statusItem;
    adjustStatusItems();
}

void PlayerItem::removeBasicStatusItem(BasicStatus *status)
{
    int i;
    for(i=0;i<statusItems.count();i++)
        if(statusItems[i]->type==0 && statusItems[i]->status==status)
        {
            delete statusItems[i];
            statusItems.removeAt(i);
        }
    adjustStatusItems();
}

void PlayerItem::addSpecialStatusItem(int specialStatusID)
{
    StatusItem *statusItem=new StatusItem(specialStatusID);
    statusItem->setParentItem(this);
    statusItems<<statusItem;
    adjustStatusItems();
}

void PlayerItem::removeSpecialStatusItem(int specialStatusID)
{
    int i;
    for(i=0;i<statusItems.count();i++)
        if(statusItems[i]->type==1 && statusItems[i]->specialStatusID==specialStatusID)
        {
            delete statusItems[i];
            statusItems.removeAt(i);
        }
    adjustStatusItems();
}

void PlayerItem::addTokenItem(Token*token)
{
    StatusItem *statusItem=new StatusItem(token);
    statusItem->setParentItem(this);
    statusItems<<statusItem;
    adjustStatusItems();
}

void PlayerItem::adjustStatusItems()
{
    int i,j;
    int left;
    int howMany=statusItems.count();
    int row=howMany/6;
    if(howMany%6!=0)
        row++;
    for(i=0;i<row;i++)
    {
        if(howMany-i*6>=6)
            left=6;
        else
            left=howMany%6;
        for(j=0;j<left;j++)
        {
            statusItems[i*6+j]->setX(0.25*width+j*(20+1));
            statusItems[i*6+j]->setY(0.5*height+i*(20+1));
        }
    }
}

Player* PlayerItem::getPlayer()
{
    return player;
}

void PlayerItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!this->selected)
    {
        selected=1;
        update();
        emit playerSelected(player->getID());
    }
    else
    {
        selected=0;
        update();
        emit playerUnselected(player->getID());
    }
}
void PlayerItem::setSelected(bool yes)
{
    selected=yes;
    update();
}
bool PlayerItem::isSelected()
{
    return selected;
}


