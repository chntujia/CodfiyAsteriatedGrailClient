#include "RoleItem.h"
#include <QPainter>
#include "GUI.h"
RoleItem::RoleItem(int roleID)
{
    this->roleID=roleID;
    pixmap.load("role/"+QString::number(roleID)+".png");
    bpFlag = -1;
    this->width=pixmap.width();
    this->height=pixmap.height();
    this->selected=0;
    setVisible(1);
}

RoleItem::~RoleItem()
{

}

QRectF RoleItem::boundingRect() const
{
    return QRectF(0, 0, width, height);
}
void RoleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0,0,pixmap);
    painter->drawPixmap(0,-10,player);
    painter->drawPixmap(0,-10,action);
}
int RoleItem::getRoleID()
{
    return this->roleID;
}
bool RoleItem::isSelected()
{
    return this->selected;
}
void RoleItem::setSelected(bool status)
{
    this->selected=status;
    update();
}

void RoleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!this->selected)
    {
        selected=1;
        setY(y()-20);
        emit roleSelected(roleID);
    }
    else
    {
        selected=0;
        setY(y()+20);
        emit roleUnselected(roleID);
    }
}

void RoleItem::ban()
{
    pixmap.load("resource/bp/Ban"+QString::number(roleID)+".png");
    update();
}

void RoleItem::setBPMsg(int playerID, int flag)
{
    BPArea* bpArea = gui->getBPArea();
    QStringList team;
    team << "Blue";
    team << "Red";
    bpFlag = flag;
    int color = bpArea->getColor(playerID);
    int num = bpArea->getOrderInTeam(playerID);
    switch(flag)
    {
    case 0:
        player.load("resource/bp/Game"+team[color]+QString::number(num)+".png");
        action.load("resource/bp/GameBan.png");
        break;
    case 1:
        player.load("resource/bp/Game"+team[color]+QString::number(num)+".png");
        action.load("resource/bp/GamePick"+team[color]+".png");
        break;
    case 2:
        player.load("resource/bp/Game"+team[color]+QString::number(num)+".png");
        action.load("resource/bp/GameInsertBan.png");
        break;
    }
    update();
}
