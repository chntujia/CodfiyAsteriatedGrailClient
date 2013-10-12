#include "MoGong.h"

MoGong::MoGong()
{
    makeConnection();
    setMyRole(this);

    Button *leiGuangSanShe, *checkCover;

    leiGuangSanShe=new Button(3,"雷光散射");
    buttonArea->addButton(leiGuangSanShe);
    connect(leiGuangSanShe,SIGNAL(buttonSelected(int)),this,SLOT(LeiGuangSanShe()));

    checkCover = new Button(10,QStringLiteral("查看充能"));
    buttonArea->addOutsideTurnButton(checkCover);
    checkCover->setVisible(true);
    checkCover->setEnabled(true);
    connect(checkCover,SIGNAL(buttonSelected(int)),gui,SLOT(showCoverArea()));
    connect(checkCover,SIGNAL(buttonUnselected(int)),gui,SLOT(closeCoverArea()));
}

void MoGong::normal()
{
    Role::normal();
    bool hasThunder=false;
    foreach(Card*ptr,dataInterface->getCoverCards())
        if(ptr->getElement()=="thunder")
        {
            hasThunder=true;
            break;
        }
    if(hasThunder && !ChongNengUsed)
        buttonArea->enable(3);
    unactionalCheck();
}

void MoGong::MoGuanChongJi()
{
    state=2601;
    gui->reset();
    tipArea->setMsg("是否发动魔贯冲击？");
    gui->showCoverArea(true);
    coverArea->enableElement("fire");
    coverArea->setQuota(1);
    decisionArea->enable(1);
}

void MoGong::MoGuanChongJiHit()
{
    state = 2602;
    gui->reset();
    tipArea->setMsg("攻击命中，额外移除1火系充能伤害加1");
    gui->showCoverArea(true);

    coverArea->enableElement("fire");
    coverArea->setQuota(1);
    buttonArea->enable(1);
    decisionArea->enable(1);
}

void MoGong::LeiGuangSanShe()
{
    state = 2603;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    tipArea->setMsg("请选择雷系充能");
    gui->showCoverArea(true);
    coverArea->reset();
    coverArea->enableElement("thunder");
    int n = 0;
    foreach(Card*ptr,dataInterface->getCoverCards())
        if(ptr->getElement()=="thunder")
            n++;
    coverArea->setQuota(1,n);
    decisionArea->enable(1);
}

void MoGong::DuoChongSheJi()
{
    state = 2605;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    tipArea->setMsg("请选择风系充能");
    gui->showCoverArea(true);
    coverArea->reset();
    coverArea->enableElement("wind");
    coverArea->setQuota(1);
    decisionArea->disable(0);
}

void MoGong::ChongNengMoYan1()
{
    state = 2661;
    gui->reset();
    tipArea->setMsg("是否发动充能/魔眼？");
    Player* myself = dataInterface->getMyself();
    if(myself->getEnergy()>0)
        tipArea->addBoxItem("1.充能");
    if(myself->getGem()>0)
        tipArea->addBoxItem("2.魔眼");
    tipArea->showBox();
    decisionArea->enable(1);
    if(myself->getEnergy()>0)
        decisionArea->enable(0);
}

void MoGong::ChongNengMoYan2()
{
    state = 2662;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    Player* myself = dataInterface->getMyself();
    if(startChoice==1)
    {
        tipArea->setMsg("弃到4张手牌，并选择摸牌数量");
        for(int i = 1; i < 5; i ++)
            tipArea->addBoxItem(QString::number(i));
        tipArea->showBox();
        if(myself->getHandCardNum()<=4)
            decisionArea->enable(0);
        else
        {
            decisionArea->disable(0);
            handArea->enableAll();
            handArea->setQuota(myself->getHandCardNum()-4);
        }
        decisionArea->enable(1);
    }
    else
    {
        tipArea->setMsg("【选择目标角色】弃1张牌或【不选择目标】你摸3张牌");
        playerArea->enableAll();
        playerArea->setQuota(1);
        decisionArea->enable(0);
        decisionArea->enable(1);
    }
}

void MoGong::ChongNengGaiPai()
{
    state = 2607;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    tipArea->setMsg("请选择最多"+QString::number(chongnengNum)+"张手牌盖做充能");
    handArea->enableAll();
    handArea->setQuota(0,chongnengNum);
    decisionArea->enable(0);
}

void MoGong::MoYanGaiPai()
{
    state = 2608;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    tipArea->setMsg(("请选择一张手牌盖做充能"));
    handArea->enableAll();
    handArea->setQuota(1);
    if(dataInterface->getHandCards().size()==0)
        decisionArea->enable(0);
}

void MoGong::cardAnalyse()
{
    Role::cardAnalyse();
    switch(state)
    {
    case 2662:
        if(startChoice==1)
            decisionArea->enable(0);
        break;
    case 2607:
        if(gui->getHandArea()->getSelectedCards().size()<gui->getHandArea()->getHandCardItems().size())
            decisionArea->enable(0);
        else
            decisionArea->disable(0);
        break;
    case 2608:
        decisionArea->enable(0);
        break;
    }
}

void MoGong::coverCardAnalyse()
{
    Role::coverCardAnalyse();
    QList<Card*> selectedCoverCards = this->coverArea->getSelectedCards();
    switch(state)
    {
    case 2601:
        if(selectedCoverCards[0]->getElement()=="fire")
            decisionArea->enable(0);
        break;
    case 2602:
        decisionArea->enable(0);
        break;
    case 2603:
        if(selectedCoverCards.size()==1)
        {
            decisionArea->enable(0);
            playerArea->disableAll();
            playerArea->setQuota(0);
        }
        else
        {
            decisionArea->disable(0);
            playerArea->enableEnemy();
            playerArea->setQuota(1);
        }
        break;
    case 2605:
        setAttackTarget();
        playerArea->disablePlayerItem(lastTarget);
        break;
    }
}

void MoGong::onOkClicked()
{
    static QString command;
    QString sourceID = QString::number(myID);
    QString targetID;
    QString text,cardID;
    Player* myself = dataInterface->getMyself();
    QList<Card*> selectedCards = handArea->getSelectedCards();
    QList<Card*> selectedCoverCards = coverArea->getSelectedCards();
    QList<Player*> selectedPlayers = playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;

    switch(state)
    {
//NORMALACTION
    case 1:
//追加行动
    case 10:
    case 11:
    case 12:
        if(selectedCards.size()>0&&selectedCards[0]->getType()=="attack")
            lastTarget = selectedPlayers[0]->getID();
        break;
//额外行动询问
    case 42:
        text=tipArea->getBoxCurrentText();
        switch (text[0].digitValue()){
        case 1:
            respond = newRespond(2604);
            respond->add_args(1);
            emit sendCommand(network::MSG_RESPOND, respond);
            DuoChongSheJi();
            break;
        }
        break;
    case 2601:
        respond = newRespond(2601);
        respond->add_args(selectedCoverCards[0]->getID());

        MoGuanChongJiUsed = true;
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 2602:
        respond = newRespond(2602);
        respond->add_args(selectedCoverCards[0]->getID());

        MoGuanChongJiUsed = true;
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 2603:
        action = newAction(2603);
        if(selectedPlayers.size()!=0)
            action->add_dst_ids(selectedPlayers[0]->getID());
        else
            targetID=QString::number(-1);
        foreach(Card*ptr,selectedCoverCards)
            action->add_args(ptr->getID());
        gui->reset();
        emit sendCommand(network::MSG_ACTION, action);
        break;
    case 2605:
        respond = newRespond(2605);
        respond->add_args(selectedCoverCards[0]->getID());
        respond->add_dst_ids(selectedPlayers[0]->getID());

        DuoChongSheJiUsed = true;
        lastTarget = selectedPlayers[0]->getID();
        usedAttack=true;
        gui->reset();
        emit sendCommand(network::MSG_RESPOND, respond);
        break;
    case 2661:
        startChoice = tipArea->getBoxCurrentText()[0].digitValue();
        ChongNengMoYan2();
        break;
    case 2662:
        respond = newRespond(2606);

        start = true;
        if(startChoice==1)
        {
            respond->add_args(1);
            foreach(Card* ptr, selectedCards)
            {
                respond->add_args(ptr->getID());
                dataInterface->removeHandCard(ptr);
            }
            respond->add_args(100000);
            chongnengNum = tipArea->getBoxCurrentText()[0].digitValue();
            ChongNengUsed = true;
        }
        else
        {
            respond->add_args(2);
            if(selectedPlayers.size()>0)
                respond->add_dst_ids(selectedPlayers[0]->getID());
        }
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 2607:
        respond = newRespond(2607);

        foreach(Card* ptr, selectedCards)
        {
            respond->add_args(ptr->getID());
            dataInterface->removeHandCard(ptr);
        }
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 2608:
        respond = newRespond(2608);

        if(selectedCards.size()==0)
            respond->add_args(100000);
        else
        {
            respond->add_args(selectedCards[0]->getID());
            dataInterface->removeHandCard(selectedCards[0]);
        }
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
    Role::onOkClicked();
}

void MoGong::onCancelClicked()
{
    Role::onCancelClicked();
    static QString command;

    network::Respond* respond;
    switch(state)
    {
    case 2601:
        MoGuanChongJiUsed = false;
        respond = newRespond(2601);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 2603:
        normal();
        break;
    case 2602:
        gui->reset();
        respond = newRespond(2601);
        emit sendCommand(network::MSG_RESPOND, respond);
        break;
    case 2605:
        DuoChongSheJi();
        break;
    case 2661:
        respond = newRespond(2606);
        emit sendCommand(network::MSG_RESPOND, respond);
        break;
    case 2662:
        ChongNengMoYan1();
        break;
    }
}

void MoGong::additionalAction()
{
    Role::additionalAction();
    if(usedAttack && !MoGuanChongJiUsed && dataInterface->getCoverCards().size()>0)
        tipArea->addBoxItem(QStringLiteral("1.多重射击"));
}

void MoGong::turnBegin()
{
    Role::turnBegin();
    MoGuanChongJiUsed = false;
    DuoChongSheJiUsed = false;
    ChongNengUsed = false;
}

void MoGong::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==QStringLiteral("充能/魔眼"))
        ChongNengMoYan1();
    else if(skill==QStringLiteral("魔贯冲击命中"))
        MoGuanChongJiHit();
    else if(skill==QStringLiteral("充能盖牌"))
        ChongNengGaiPai();
    else if(skill==QStringLiteral("魔眼盖牌"))
        MoYanGaiPai();
    else if(skill==QStringLiteral("魔贯冲击"))
        MoGuanChongJi();
}
