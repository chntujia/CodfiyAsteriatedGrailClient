#include "BPArea.h"
#include <QPainter>
#include <QFont>
#include "data/DataInterface.h"
#include "widget/GUI.h"

static QRectF BPAreaRect(0, 0, 1000, 1000);
static const QPointF RolePos[]=
{
    QPointF(160,230),QPointF(234,230),QPointF(320,230),QPointF(400,230),
    QPointF(480,230),QPointF(560,230),QPointF(640,230),QPointF(720,230),
    QPointF(160,500),QPointF(240,500),QPointF(320,500),QPointF(400,500),
    QPointF(480,500),QPointF(560,500),QPointF(640,500),QPointF(720,500),
};
BPArea::BPArea():least(1),most(1)
{
    height=1000;
    width=1000;
    setVisible(false);
    currentSum = 0;
}

void BPArea::BPStart(int num, SafeList<int> roles)
{
/*    QString queue = dataInterface->getQueue();
    int red =0, blue = 0, max = queue.size()/2;
    for(int i =0;i<max;i++)
    {
        playerIDs << queue[i].digitValue();
        color << queue[i+max].digitValue();
        if(queue[i+max].digitValue()==1)
            orderInTeam << ++red;
        else
            orderInTeam << ++blue;
    }
    gui->reset();
     left = roles;
    currentSum = 0;
    for(int i=0;i<num;i++)
    {
        roleItems << new RoleItem(roles[i]);
        connect(roleItems[i],SIGNAL(roleSelected(int)),this,SLOT(onRoleSelected(int)));
        connect(roleItems[i],SIGNAL(roleUnselected(int)),this,SLOT(onRoleUnselected(int)));
        if(i<8)
        {
            roleItems[i]->setX(195+65*i);
            roleItems[i]->setY(230);
        }
        else
        {
            roleItems[i]->setX(195+65*(i-8));
            roleItems[i]->setY(480);
        }
        roleItems[i]->setParentItem(this);
    }
    setVisible(true);
    reset();
    */
}

void BPArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QFont font;
    font.setPixelSize(20);
    painter->setFont(font);
    painter->setPen(QColor(Qt::white));
    painter->drawText(QRectF(0, 0, width, height*0.85),Qt::AlignCenter,msg);
}

QRectF BPArea::boundingRect() const
{
    return BPAreaRect;
}

void BPArea::enableRoleItem(int roleID)
{
    RoleItem* role = getRoleByID(roleID);
    role->setEnabled(1);
    role->setOpacity(1);
}

void BPArea::disableRoleItem(int roleID)
{
    RoleItem* role = getRoleByID(roleID);
    role->setEnabled(0);
    role->setOpacity(0.8);
}

void BPArea::onRoleSelected(int id)
{
    if(selectedRoles.count()>=most)
    {
        int firstRole=selectedRoles.takeFirst();
        RoleItem* tmp = getRoleByID(firstRole);
        tmp->setSelected(0);
        tmp->setY(tmp->y()+20);
    }
    selectedRoles<<id;
    if(selectedRoles.count()<least)
        return;
    emit roleReady();
}

void BPArea::onRoleUnselected(int id)
{
    selectedRoles.removeOne(id);
    if(selectedRoles.count()<least)
    {
        gui->getDecisionArea()->disable(0);
        emit roleUnready();
    }
}

void BPArea::setMsg(QString msg)
{
    this->msg=msg;
    setVisible(1);
    update();
}

SafeList<int> BPArea::getSelectedRoles()
{
    return selectedRoles;
}

RoleItem *BPArea::getRoleByID(int ID)
{
    foreach(RoleItem* ptr, roleItems)
    {
        if(ptr->getRoleID() ==ID)
            return ptr;
    }
return 0;
}

void BPArea::choose(int playerID, int roleID)
{
    currentSum++;
    RoleItem* choice = getRoleByID(roleID);
    choice->setEnabled(0);
    choice->setOpacity(0.8);
    choice->setBPMsg(playerID,1);
    remove(roleID);
}

void BPArea::ban(int playerID, int roleID)
{
    RoleItem* ban = getRoleByID(roleID);
    ban->setEnabled(0);
    ban->ban();
    ban->setBPMsg(playerID,0);
    remove(roleID);
}


void BPArea::reset()
{
    selectedRoles.clear();
    setMsg("");
    disablaAll();
}


bool BPArea::checkOver()
{
    int max = dataInterface->getPlayerMax();
    if(currentSum==max)
        return true;
    return false;
}

void BPArea::remove(int roleID)
{
    left.removeOne(roleID);
}

void BPArea::disablaAll()
{
    foreach(int ptr, left)
    {
        getRoleByID(ptr)->setEnabled(0);
    }
}

void BPArea::enableAll()
{
    foreach(int ptr, left)
    {
        getRoleByID(ptr)->setEnabled(1);
    }
}

int BPArea::getColor(int playerID)
{
    for(int i=0;i<playerIDs.size();i++)
        if(playerIDs[i]==playerID)
            return color[i];
return 0;
}

int BPArea::getOrderInTeam(int playerID)
{
    for(int i=0;i<playerIDs.size();i++)
        if(playerIDs[i]==playerID)
            return orderInTeam[i];
return 0;
}
