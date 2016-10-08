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
    justShow = true;
}

BPArea::~BPArea()
{
    for(int i=0;i<roleItems.size();i++)
    {
        RoleItem * item = roleItems[i];
        disconnect(item,SIGNAL(roleSelected(int)),this,SLOT(onRoleSelected(int)));
        disconnect(item,SIGNAL(roleUnselected(int)),this,SLOT(onRoleUnselected(int)));
        delete item;
    }
}

void BPArea::BPStart(int num, SafeList<int> roles, SafeList<int> options, uint op)
{
    //QString queue = dataInterface->get;
    int red =0, blue = 0, max = roles.size()/2;
    for(int i =0;i<max;i++)
    {
        playerIDs << roles[i];
        color << options[i];
        if(roles[i+max] ==1)
            orderInTeam << ++red;
        else
            orderInTeam << ++blue;
    }
    gui->reset();
    selectedRoles.clear();
    bool needCreate = (roleItems.size() == 0);
    for(int i=0;i<num;i++)
    {
        RoleItem * item = NULL;
        if(needCreate)
        {
            item = new RoleItem(roles[i]);
            roleItems << item;
        }
        else
        {
            item = roleItems[i];
        }
        item->setBPMsg(options[i]);
        if(op == BP_NULL)
        {
            justShow = true;
            disconnect(item,SIGNAL(roleSelected(int)),this,SLOT(onRoleSelected(int)));
            disconnect(item,SIGNAL(roleUnselected(int)),this,SLOT(onRoleUnselected(int)));
        }
        else
        {
            justShow = false;
            connect(item,SIGNAL(roleSelected(int)),this,SLOT(onRoleSelected(int)));
            connect(item,SIGNAL(roleUnselected(int)),this,SLOT(onRoleUnselected(int)));
        }
        if(i<8)
        {
            item->setX(195+65*i);
            item->setY(230);
        }
        else
        {
            item->setX(195+65*(i-8));
            item->setY(480);
        }
        item->setParentItem(this);
    }
    setVisible(true);
    reset();
}
void BPArea::CMStart(int num, SafeList<int> roles, SafeList<int> options, uint op)
{
    //QString queue = dataInterface->get;
    int red =0, blue = 0, max = roles.size()/2;
    for(int i =0;i<max;i++)
    {
        playerIDs << roles[i];
        color << options[i];
        if(roles[i+max] ==1)
            orderInTeam << ++red;
        else
            orderInTeam << ++blue;
    }
    gui->reset();
    selectedRoles.clear();
    bool needCreate = (roleItems.size() == 0);
    for(int i=0;i<num;i++)
    {
        RoleItem * item = NULL;
        if(needCreate)
        {
            item = new RoleItem(roles[i]);
            roleItems << item;
        }
        else
        {
            item = roleItems[i];
        }
        item->setCMMsg(options[i]);
        if(op == CM_NULL)
        {
            justShow = true;
            disconnect(item,SIGNAL(roleSelected(int)),this,SLOT(onRoleSelected(int)));
            disconnect(item,SIGNAL(roleUnselected(int)),this,SLOT(onRoleUnselected(int)));
        }
        else
        {
            justShow = false;
            connect(item,SIGNAL(roleSelected(int)),this,SLOT(onRoleSelected(int)));
            connect(item,SIGNAL(roleUnselected(int)),this,SLOT(onRoleUnselected(int)));
        }
        if(i<8)
        {
            item->setX(195+65*i);
            item->setY(230);
        }
        else
        {
            item->setX(195+65*(i-8));
            item->setY(480);
        }
        item->setParentItem(this);
    }
    setVisible(true);
    reset();
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
    role->setOpacity(0.5);
}

void BPArea::onRoleSelected(int id)
{
    if(justShow)
        return;
    RoleItem* tmp;
    if(selectedRoles.count()>=most)
    {
        int firstRole=selectedRoles.takeFirst();
        tmp = getRoleByID(firstRole);
        tmp->setSelected(false);
    }
    selectedRoles<<id;
    tmp = getRoleByID(id);
    tmp->setSelected(true);
    if(selectedRoles.count()<least)
        return;
    emit roleReady();
}

void BPArea::onRoleUnselected(int id)
{
    selectedRoles.removeOne(id);
    RoleItem* tmp;
    tmp = getRoleByID(id);
    tmp->setSelected(false);
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
    choice->setBPMsg(playerID);
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

void BPArea::hide()
{
    RoleItem * item;
    for(int i=0;i<roleItems.size();i++)
    {
        item = roleItems[i];
        item->setVisible(false);
    }
    setVisible(false);
}
