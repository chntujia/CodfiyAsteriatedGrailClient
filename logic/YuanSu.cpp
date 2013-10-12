#include "YuanSu.h"

YuanSu::YuanSu()
{
    makeConnection();
setMyRole(this);

    Button *yuanSuFaShu,*yuanSuDianRan,*yueGuang;
    yuanSuFaShu=new Button(3,QStringLiteral("元素法术"));
    buttonArea->addButton(yuanSuFaShu);
    connect(yuanSuFaShu,SIGNAL(buttonSelected(int)),this,SLOT(YuanSuFaShu1()));

    yuanSuDianRan=new Button(4,QStringLiteral("元素点燃"));
    buttonArea->addButton(yuanSuDianRan);
    connect(yuanSuDianRan,SIGNAL(buttonSelected(int)),this,SLOT(YuanSuDianRan()));

    yueGuang=new Button(5,QStringLiteral("月光"));
    buttonArea->addButton(yueGuang);
    connect(yueGuang,SIGNAL(buttonSelected(int)),this,SLOT(YueGuang()));
}

void YuanSu::YuanSuFaShu1()
{
    state=1100;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    tipArea->setMsg(QStringLiteral("请先选择咏系牌和法伤目标"));
    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableProperty(QStringLiteral("咏"));
    handArea->disableElement("darkness");
    handArea->disableMagic();
}

void YuanSu::YuanSuFaShu2()
{
    state=1101;
    magicCard=handArea->getSelectedCards().takeFirst();
    magicTargetID=playerArea->getSelectedPlayers().takeFirst()->getID();

    playerArea->reset();
    handArea->reset();
    handArea->setQuota(0,1);
    handArea->enableElement(magicCard->getElement());
    foreach(CardItem*ptr, handArea->getHandCardItems())
        if(ptr->getCard()->getID()==magicCard->getID())
        {
            ptr->setEnabled(0);
            ptr->setY(-20);
        }

    if(magicCard->getElement()=="water")
    {
        playerArea->enableAll();        
        decisionArea->disable(0);
        tipArea->setMsg(QStringLiteral("请选择治疗目标；若多弃1张同系牌，本次伤害+1"));
    }
    else
        tipArea->setMsg(QStringLiteral("若多弃1张同系牌，本次伤害+1"));
}

void YuanSu::YuanSuDianRan()
{
    state=1102;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    playerArea->enableAll();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void YuanSu::YueGuang()
{
    state=1103;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    playerArea->enableAll();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void YuanSu::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
//元素法术
    foreach(Card*ptr, dataInterface->getHandCards())
        if (ptr->getType()=="attack"&&ptr->getProperty()==QStringLiteral("咏")&&ptr->getElement()!="darkness")
            buttonArea->enable(3);
//元素点燃
    if(myself->getToken(0)==3)
        buttonArea->enable(4);
//月光
    if(myself->getGem()>0)
        buttonArea->enable(5);
    unactionalCheck();
}

void YuanSu::magicAction()
{
    Role::magicAction();
    Player* myself=dataInterface->getMyself();
//元素法术
    foreach(Card*ptr, dataInterface->getHandCards())
        if (ptr->getType()=="attack"&&ptr->getProperty()==QStringLiteral("咏")&&ptr->getElement()!="darkness")
            buttonArea->enable(3);
//元素点燃
    if(myself->getToken(0)==3)
        buttonArea->enable(4);
//月光
    if(myself->getGem()>0)
        buttonArea->enable(5);
}

void YuanSu::cardAnalyse()
{
    Role::cardAnalyse();

    switch (state)
    {
//元素法术1
    case 1100:
        playerArea->enableAll();
        break;
    }
}

void YuanSu::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    QString command;
    QString cardID;
    QString sourceID;
    QString targetID;
    QString text;    
    bool water=false;
    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;

    switch(state)
    {
//额外行动询问
    case 42:
        text=tipArea->getBoxCurrentText();
        switch(text[0].digitValue())
        {
        case 1:
            earth=false;
            respond = newRespond(1105);
            respond->add_args(1);
            emit sendCommand(network::MSG_RESPOND, respond);
            magicAction();
            break;
        case 2:
            ignite=false;
            respond = newRespond(1106);
            respond->add_args(1);
            emit sendCommand(network::MSG_RESPOND, respond);
            magicAction();
            break;
        case 3:
            wind=false;
            respond = newRespond(1104);
            respond->add_args(1);
            emit sendCommand(network::MSG_RESPOND, respond);
            attackAction();
            break;
        }
        break;
//元素法术1
    case 1100:
        YuanSuFaShu2();
        break;
//元素法术2
    case 1101:
        action = newAction(1101);
        if(magicCard->getElement()=="wind"){
            action->add_args(1);
            wind=true;
        }
        else if(magicCard->getElement()=="water"){
            action->add_args(2);
            water=true;
        }
        else if(magicCard->getElement()=="fire")
            action->add_args(3);
        else if(magicCard->getElement()=="earth"){
            action->add_args(4);
            earth=true;
        }
        else
            action->add_args(5);
        action->add_args(magicCard->getID());
        action->add_dst_ids(magicTargetID);
        if(handArea->getSelectedCards().size()>0){
            action->add_args(selectedCards[0]->getID());
            dataInterface->removeHandCard(selectedCards[0]);
        }
        if(water)
            action->add_dst_ids(selectedPlayers[0]->getID());
        dataInterface->removeHandCard(magicCard);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//元素点燃
    case 1102:
        action = newAction(1102);
        action->add_args(1);
        action->add_dst_ids(selectedPlayers[0]->getID());
        ignite=true;
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//月光
    case 1103:
        action = newAction(1103);
        action->add_args(1);
        action->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    }
}

void YuanSu::onCancelClicked()
{
    Role::onCancelClicked();
    switch(state)
    {
    case 1100:
    case 1102:
    case 1103:
        if(actionFlag==0)
            normal();
        else if(actionFlag==2)
            magicAction();
        break;
    case 1101:
        YuanSuFaShu1();
        break;
    }
}
void YuanSu::additionalAction()
{
    Role::additionalAction();
    if(earth)
        tipArea->addBoxItem(QStringLiteral("1.法术行动（陨石）"));
    if(ignite)
        tipArea->addBoxItem(QStringLiteral("2.法术行动（元素点燃）"));
    if(wind)
        tipArea->addBoxItem(QStringLiteral("3.攻击行动（风刃）"));
}

void YuanSu::turnBegin()
{
    Role::turnBegin();
    wind=false;
    earth=false;
    ignite=false;
}
