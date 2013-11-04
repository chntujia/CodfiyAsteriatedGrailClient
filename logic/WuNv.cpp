#include "WuNv.h"

WuNv::WuNv()
{
    makeConnection();
    setMyRole(this);
    tongShengID = -1;
    Button *tongShengGongSi, *niLiu, *xueZhiBeiMing, *xueZhiZuZhou;
    tongShengGongSi = new Button(3,QStringLiteral("同生共死"));
    buttonArea->addButton(tongShengGongSi);
    connect(tongShengGongSi,SIGNAL(buttonSelected(int)),this,SLOT(TongShengGongSi()));

    niLiu = new Button(4,QStringLiteral("逆流"));
    buttonArea->addButton(niLiu);
    connect(niLiu,SIGNAL(buttonSelected(int)),this,SLOT(NiLiu()));

    xueZhiBeiMing = new Button(5,QStringLiteral("血之悲鸣"));
    buttonArea->addButton(xueZhiBeiMing);
    connect(xueZhiBeiMing,SIGNAL(buttonSelected(int)),this,SLOT(XueZhiBeiMing()));

    xueZhiZuZhou = new Button(6,QStringLiteral("血之诅咒"));
    buttonArea->addButton(xueZhiZuZhou);
    connect(xueZhiZuZhou,SIGNAL(buttonSelected(int)),this,SLOT(XueZhiZuZhou()));
}

void WuNv::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    if(tongShengID==-1)
        buttonArea->enable(3);
    if(myself->getTap())
        buttonArea->enable(4);
    if(myself->getTap()&&handArea->checkSpecility(QStringLiteral("血之悲鸣")))
        buttonArea->enable(5);
    if(myself->getGem()>0)
        buttonArea->enable(6);
    unactionalCheck();
}

void WuNv::TongShengGongSi()
{
    state = 2301;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    playerArea->enableAll();

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void WuNv::XueZhiAiShang()
{
    state = 2302;
    gui->reset();
    tipArea->setMsg(QStringLiteral("点选目标转移同生共死或直接确定移除"));
    playerArea->setQuota(0,1);
    playerArea->enableAll();
    if(tongShengID!=-1)
        playerArea->disablePlayerItem(tongShengID);

    decisionArea->enable(0);
    decisionArea->enable(1);
}

void WuNv::NiLiu()
{
    state = 2303;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    handArea->setQuota(2);
    handArea->enableAll();

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void WuNv::XueZhiBeiMing()
{
    state = 2304;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    tipArea->setMsg("请选择【血之悲鸣】伤害");
    for(int i =3;i>0;i--)
        tipArea->addBoxItem(QString::number(i));
    tipArea->showBox();

    handArea->setQuota(1);
    handArea->enableSpecility(QStringLiteral("血之悲鸣"));
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void WuNv::XueZhiZuZhou()
{
    state = 2305;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->enableAll();

    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void WuNv::XueZhiZuZhouQiPai()
{
    state = 2306;
    gui->reset();
    tipArea->setMsg(QStringLiteral("请弃3张手牌（不足则全弃）"));
    QList<Card*> handcards=dataInterface->getHandCards();
    if(handcards.size()>=3)
        handArea->setQuota(3);
    else
        handArea->setQuota(handcards.size());
    handArea->enableAll();
}

void WuNv::cardAnalyse()
{
    Role::cardAnalyse();
    switch(state)
    {
    case 2301:
        playerArea->enableAll();
        break;
    case 2303:
        decisionArea->enable(0);
        break;
    case 2304:
        playerArea->enableAll();
        break;
    case 2306:
        decisionArea->enable(0);
        break;
    }
}

void WuNv::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    static QString command;
    QString cardID;
    QString sourceID;
    QString targetID;
    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;
    switch(state)
    {
    case 2301:
        action = newAction(2301);
        action->add_args(1);
        action->add_dst_ids(selectedPlayers[0]->getID());
        tongShengID = selectedPlayers[0]->getID();
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case 2302:
        respond = newRespond(2302);
        respond->add_args(1);
        start = true;
        if(selectedPlayers.size()!=0)
        {
            respond->add_dst_ids(selectedPlayers[0]->getID());
            tongShengID = selectedPlayers[0]->getID();
        }
        else
        {
            tongShengID = -1;
        }
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 2303:
        action = newAction(2303);
        foreach(Card*ptr,selectedCards){
            action->add_args(ptr->getID());
            dataInterface->removeHandCard(ptr);
        }
        action->add_args(100000);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case 2304:
        action = newAction(2304);
        action->add_dst_ids(selectedPlayers[0]->getID());
        action->add_args(selectedCards[0]->getID());
        action->add_args(tipArea->getBoxCurrentText().toInt());
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case 2305:
        action = newAction(2305);
        action->add_dst_ids(selectedPlayers[0]->getID());
        action->add_args(1);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case 2306:
        respond = newRespond(2306);
        foreach(Card*ptr,selectedCards)
        {
            respond->add_args(ptr->getID());
            dataInterface->removeHandCard(ptr);
        }
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
}

void WuNv::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;

    network::Respond* respond;
    switch(state)
    {
    case 2301:
    case 2303:
    case 2304:
    case 2305:
        normal();
        break;
    case 2302:
        respond = newRespond(2302);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
}

void WuNv::askForSkill(QString skill)
{
    //Role::askForSkill(skill);
    if(skill==QStringLiteral("血之哀伤"))
        XueZhiAiShang();
    else if(skill==QStringLiteral("血之诅咒弃牌"))
        XueZhiZuZhouQiPai();
}
