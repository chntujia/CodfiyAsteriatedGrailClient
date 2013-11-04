#include "QiDao.h"

QiDao::QiDao()
{
    makeConnection();
    setMyRole(this);

    Button *weiLi,*xunJie,*guangHui,*qiHei;
    weiLi=new Button(3,QStringLiteral("威力赐福"));
    buttonArea->addButton(weiLi);
    connect(weiLi,SIGNAL(buttonSelected(int)),this,SLOT(WeiLiCiFu()));

    xunJie=new Button(4,QStringLiteral("迅捷赐福"));
    buttonArea->addButton(xunJie);
    connect(xunJie,SIGNAL(buttonSelected(int)),this,SLOT(XunJieCiFu()));

    guangHui=new Button(5,QStringLiteral("光辉信仰"));
    buttonArea->addButton(guangHui);
    connect(guangHui,SIGNAL(buttonSelected(int)),this,SLOT(GuangHuiXinYang()));

    qiHei=new Button(6,QStringLiteral("漆黑信仰"));
    buttonArea->addButton(qiHei);
    connect(qiHei,SIGNAL(buttonSelected(int)),this,SLOT(QiHeiXinYang()));
}

void QiDao::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();

    if (handArea->checkSpecility(QStringLiteral("威力赐福")))
        buttonArea->enable(3);
    if (handArea->checkSpecility(QStringLiteral("迅捷赐福")))
        buttonArea->enable(4);
    if(myself->getTap()&&myself->getToken(0)>0)
    {
        buttonArea->enable(5);
        buttonArea->enable(6);
    }
    unactionalCheck();
}

void QiDao::magicAction()
{
    Role::magicAction();
    Player* myself=dataInterface->getMyself();

    if (handArea->checkSpecility(QStringLiteral("威力赐福")))
        buttonArea->enable(3);
    if (handArea->checkSpecility(QStringLiteral("迅捷赐福")))
        buttonArea->enable(4);
    if(myself->getTap()&&myself->getToken(0)>0)
    {
        buttonArea->enable(5);
        buttonArea->enable(6);
    }
}

void QiDao::QiDong()
{
    state=1601;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动祈祷？"));
    QList<Card*> handcards=dataInterface->getHandCards();
    bool flag=true;
    int i;
    int n=handcards.size();
    decisionArea->enable(1);
    if(n<4)
    {
        flag=false;
        for(i=0;i<n;i++)
            if(handcards[i]->getElement()!="light")
            {
                flag=true;
                break;
            }
    }
    if(flag)
        decisionArea->enable(0);
}

void QiDao::WeiLiCiFu()
{
    state=1602;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableSpecility(QStringLiteral("威力赐福"));
}

void QiDao::XunJieCiFu()
{
    state=1603;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableSpecility(QStringLiteral("迅捷赐福"));
}

void QiDao::GuangHuiXinYang()
{
    state=1604;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    int howMany=handArea->getHandCardItems().size();
    howMany=2<howMany?2:howMany;
    handArea->setQuota(howMany);
    handArea->enableAll();
    if(howMany==0)
        playerArea->enableMate();
    decisionArea->enable(1);
    decisionArea->disable(0);
}

void QiDao::QiHeiXinYang()
{
    state=1605;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    playerArea->enableAll();

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void QiDao::cardAnalyse()
{
    Role::cardAnalyse();
    QList<Player*>players=dataInterface->getPlayerList();
    switch (state)
    {
//威力赐福
    case 1602:
        playerArea->enableMate();
        for(int i=0;i<players.size();i++)
           if(players[i]->checkBasicStatus(4))
               playerArea->disablePlayerItem(i);
        break;
//迅捷赐福
    case 1603:
        playerArea->enableMate();
        for(int i=0;i<players.size();i++)
           if(players[i]->checkBasicStatus(5))
               playerArea->disablePlayerItem(i);
        break;
//光辉信仰
    case 1604:
        playerArea->enableMate();
        break;
    }
}

void QiDao::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    QString command;
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
//额外行动询问
    case 42:
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1'){
            onceUsed=true;

            respond = newRespond(1606);
            respond->add_args(1);
            emit sendCommand(network::MSG_RESPOND, respond);
            magicAction();

        }
        break;
//祈祷
    case 1601:
        respond = newRespond(1606);
        respond->add_args(1);
        start = true;
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//威力赐福
    case 1602:
        action = newAction(1602);
        action->add_args(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());

        dataInterface->removeHandCard(selectedCards[0]);
        usedMagic=true;
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//迅捷赐福
    case 1603:
        action = newAction(1603);
        action->add_args(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());

        dataInterface->removeHandCard(selectedCards[0]);
        usedMagic=true;
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//光辉信仰
    case 1604:
        action = newAction(1604);
        foreach(Card*ptr,selectedCards){
            action->add_args(ptr->getID());
            dataInterface->removeHandCard(ptr);
        }
        action->add_dst_ids(selectedPlayers[0]->getID());

        usedMagic=true;
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//漆黑信仰
    case 1605:
        action = newAction(1605);
        action->add_args(1);
        action->add_dst_ids(selectedPlayers[0]->getID());

        usedMagic=true;
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    }
}

void QiDao::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;

    network::Respond* respond;

    switch(state)
    {
//祈祷
    case 1601:
        respond = newRespond(1606);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//威力赐福
    case 1602:
//迅捷赐福
    case 1603:
//光辉信仰
    case 1604:
//漆黑信仰
    case 1605:
        if(actionFlag==0)
            normal();
        else if(actionFlag==2)
            magicAction();
        break;
    }
}
void QiDao::askForSkill(QString skill)
{
    //Role::askForSkill(skill);
    if(skill==QStringLiteral("祈祷"))
        QiDong();
}
void QiDao::additionalAction()
{
    //Role::additionalAction();
    if(usedMagic && dataInterface->getMyself()->getEnergy()>0 && !onceUsed)
        tipArea->addBoxItem(QStringLiteral("1.法力潮汐"));
}
