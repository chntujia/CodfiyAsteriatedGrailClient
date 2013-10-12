#include "DieWu.h"

DieWu::DieWu()
{
    makeConnection();
    setMyRole(this);
    Button *wuDong, *yongHua, *daoNiZhiDie, *checkCover;
    wuDong = new Button(3,QStringLiteral("舞动"));
    buttonArea->addButton(wuDong);
    connect(wuDong,SIGNAL(buttonSelected(int)),this,SLOT(WuDong1()));

    yongHua = new Button(4,QStringLiteral("蛹化"));
    buttonArea->addButton(yongHua);
    connect(yongHua,SIGNAL(buttonSelected(int)),this,SLOT(YongHua()));

    daoNiZhiDie = new Button(5,QStringLiteral("倒逆之蝶"));
    buttonArea->addButton(daoNiZhiDie);
    connect(daoNiZhiDie,SIGNAL(buttonSelected(int)),this,SLOT(DaoNiZhiDie1()));

    checkCover = new Button(10,QStringLiteral("查看茧"));
    buttonArea->addOutsideTurnButton(checkCover);
    checkCover->setVisible(true);
    checkCover->setEnabled(true);
    connect(checkCover,SIGNAL(buttonSelected(int)),gui,SLOT(showCoverArea()));
    connect(checkCover,SIGNAL(buttonUnselected(int)),gui,SLOT(closeCoverArea()));
}

void DieWu::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    buttonArea->enable(3);
    if(myself->getGem()>0)
        buttonArea->enable(4);
    if(myself->getEnergy()>0)
        buttonArea->enable(5);
    unactionalCheck();
}

void DieWu::WuDong1()
{
    state = 2411;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->enable(0);

    tipArea->setMsg(QStringLiteral("请先选择一项："));
    tipArea->addBoxItem(QStringLiteral("1.摸1张牌【强制】"));
    if(dataInterface->getHandCards().size()>0)
        tipArea->addBoxItem(QStringLiteral("2.弃1张牌【强制】"));
    tipArea->showBox();
}

void DieWu::WuDong2()
{
    state = 2412;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    handArea->setQuota(1);
    handArea->enableAll();

    decisionArea->enable(1);
}

void DieWu::DuFen()
{
    state = 2402;
    tipArea->setMsg("是否发动毒粉？");

    gui->showCoverArea(true);
    HandArea *coverArea = gui->getCoverArea();
    coverArea->reset();
    coverArea->enableAll();
    coverArea->setQuota(1);
    decisionArea->enable(1);
}

void DieWu::ChaoSheng()
{
    state = 2403;
    tipArea->setMsg("是否发动朝圣？");

    gui->showCoverArea(true);
    HandArea *coverArea = gui->getCoverArea();
    coverArea->reset();
    coverArea->enableAll();
    coverArea->setQuota(1);
    decisionArea->enable(1);
}

void DieWu::JingHuaShuiYue()
{
    state = 2404;
    tipArea->setMsg("是否发动镜花水月？");

    gui->showCoverArea(true);
    HandArea *coverArea = gui->getCoverArea();
    coverArea->reset();
    coverArea->enableAll();
    coverArea->setQuota(2);
    decisionArea->enable(1);
}

void DieWu::DiaoLing()
{
    state = 2405;
    if(diaoLingFlag)
    {
        tipArea->setMsg("是否发动凋零？");
        playerArea->enableAll();
        playerArea->setQuota(1);
    }
    else
    {
        tipArea->setMsg("非法术茧，无法发动凋零，请选择取消");
    }
    decisionArea->enable(1);
}

void DieWu::YongHua()
{
    state = 2406;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void DieWu::DaoNiZhiDie1()
{
    state = 2471;
    handArea->reset();
    playerArea->reset();
    decisionArea->reset();
    tipArea->reset();
    tipArea->setMsg("你弃2张牌，再选择一项：");
    tipArea->addBoxItem("1.对目标角色造成1点法术伤害，该伤害不能用治疗抵御");
    if(dataInterface->getMyself()->getToken(2)>=2)
        tipArea->addBoxItem("2.（移除2个【茧】）移除1个【蛹】");
    tipArea->addBoxItem("3.（自己造成4点法术伤害③）移除1个【蛹】");
    tipArea->showBox();
    QList<Card*> handcards=dataInterface->getHandCards();
    if(handcards.size()>1)
        handArea->setQuota(2);
    else if(handcards.size()==1)
        handArea->setQuota(1);
    handArea->enableAll();
    decisionArea->enable(1);
    if(handcards.size()==0)
        decisionArea->enable(0);
}

void DieWu::DaoNiZhiDie2()
{
    state = 2472;
    playerArea->reset();
    tipArea->reset();

    switch(daoNiZhiDieFlag)
    {
    case 1:
        playerArea->enableAll();
        playerArea->setQuota(1);
        break;
    case 2:
        gui->showCoverArea(true);
        coverArea->reset();
        coverArea->enableAll();
        coverArea->setQuota(2);
        break;
    }
    decisionArea->disable(0);
}

void DieWu::onOkClicked()
{
    Role::onOkClicked();
    static QString command;
    QString sourceID = QString::number(myID);
    QString targetID;
    QString text,cardID;
    QList<Card*> selectedCards = handArea->getSelectedCards();
    QList<Card*> selectedCoverCards = coverArea->getSelectedCards();
    QList<Player*> selectedPlayers = playerArea->getSelectedPlayers();

    network::Action *action;
    network::Respond *respond;

    switch(state)
    {
    case 2411:
        if(tipArea->getBoxCurrentText()[0].digitValue()==1)
        {
            wudongmopai = true;
            action = new network::Action();
            action->set_src_id(myID);
            action->set_action_id(2401);
            action->add_args(0);
            emit sendCommand(network::MSG_ACTION, action);
            gui->reset();
        }
        else
        {
            wudongmopai = false;
            WuDong2();
        }
        break;
    case 2412:
        action = new network::Action();
        action->set_src_id(myID);
        action->set_action_id(2401);
        action->add_args(1);

        if(dataInterface->getHandCards().size()!=0)
        {
            action->add_args(selectedCards[0]->getID());
            dataInterface->removeHandCard(selectedCards[0]);
        }
        else
            cardID = "-1";

        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case 2402:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(2402);
        //respond->add_dst_ids(sourceID);
        respond->add_args(selectedCoverCards[0]->getID());

        dataInterface->removeCoverCard(selectedCoverCards[0]);
        coverArea->reset();
        gui->showCoverArea(false);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 2403:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(2403);
        respond->add_args(selectedCoverCards[0]->getID());

        dataInterface->removeCoverCard(selectedCoverCards[0]);
        coverArea->reset();
        gui->showCoverArea(false);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 2404:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(2404);
        respond->add_args(selectedCoverCards[0]->getID());
        respond->add_args(selectedCoverCards[1]->getID());

        dataInterface->removeCoverCard(selectedCoverCards[0]);
        dataInterface->removeCoverCard(selectedCoverCards[1]);
        coverArea->reset();
        gui->showCoverArea(false);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 2405:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(2405);
        respond->add_dst_ids(selectedPlayers[0]->getID());

        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 2406:
        action = new network::Action();
        action->set_src_id(myID);
        action->set_action_id(2406);

        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case 2471:
        daoNiZhiDieFlag = tipArea->getBoxCurrentText()[0].digitValue();
        if(daoNiZhiDieFlag == 3)
        {
            action = new network::Action();
            action->set_src_id(myID);
            action->set_action_id(2407);

            action->add_args(daoNiZhiDieFlag);

            foreach(Card*ptr,selectedCards){
                action->add_args(ptr->getID());
                dataInterface->removeHandCard(ptr);
            }

            emit sendCommand(network::MSG_ACTION, action);
            gui->reset();
        }
        else
            DaoNiZhiDie2();
        break;
    case 2472:
        action = new network::Action();
        action->set_src_id(myID);
        action->set_action_id(2407);

        action->add_args(daoNiZhiDieFlag);

        foreach(Card*ptr,selectedCards){
            action->add_args(ptr->getID());
            dataInterface->removeHandCard(ptr);
        }
        for(int i=0;i<(2-selectedCards.size());i++)
            action->add_args(10000);

        if(daoNiZhiDieFlag==1)
        {
            action->add_dst_ids(selectedPlayers[0]->getID());
        }
        else
        {
            foreach(Card*ptr,selectedCards){
                action->add_args(ptr->getID());
                dataInterface->removeHandCard(ptr);
            }

            coverArea->reset();
            gui->showCoverArea(false);
        }
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();;
        break;
    }
}

void DieWu::onCancelClicked()
{
    Role::onCancelClicked();

    network::Action *action;
    network::Respond *respond;

    switch(state)
    {
    case 2411:
    case 2406:
    case 2471:
        normal();
        break;
    case 2402:
        respond = newRespond(2402);

        coverArea->reset();
        gui->showCoverArea(false);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 2403:
        respond = newRespond(2403);

        coverArea->reset();
        gui->showCoverArea(false);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 2404:
        respond = newRespond(2404);

        coverArea->reset();
        gui->showCoverArea(false);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 2405:
        respond = newRespond(2405);

        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 2412:
        WuDong1();
        break;
    case 2472:
        DaoNiZhiDie1();
        coverArea->reset();
        gui->showCoverArea(false);
        break;
    }
}

void DieWu::cardAnalyse()
{
    Role::cardAnalyse();
    switch(state)
    {
    case 2412:
        decisionArea->enable(0);
        break;
    case 2471:
        decisionArea->enable(0);
        break;
    }
}

void DieWu::coverCardAnalyse()
{
    Role::coverCardAnalyse();
    QList<Card*> selectedCoverCards = this->coverArea->getSelectedCards();
    switch(state)
    {
    case 2402:
    case 2403:
    case 2407:
    case 2472:
        decisionArea->enable(0);
        break;
    case 2404:
        if(selectedCoverCards[0]->getElement()!=selectedCoverCards[1]->getElement())
            decisionArea->disable(0);
        else
            decisionArea->enable(0);
        break;
    }
}

void DieWu::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==QStringLiteral("毒粉"))
        DuFen();
    else if(skill==QStringLiteral("朝圣"))
        ChaoSheng();
    else if(skill==QStringLiteral("镜花水月"))
        JingHuaShuiYue();
}

void DieWu::decipher(uint16_t proto_type, google::protobuf::Message* proto)
{
    if (proto_type == network::MSG_CMD_REQ)
    {
        google::protobuf::Message* proto2 = new network::CommandRequest();
        proto2->CopyFrom(*proto);
        Role::decipher(proto_type, proto2);

        network::CommandRequest* cmd_req = (network::CommandRequest*)proto;
        if (cmd_req->cmd_type() == network::CMD_RESPOND)
        {
            for (int i = 0; i < cmd_req->commands_size(); ++i)
            {
                network::Command* cmd = (network::Command*)&(cmd_req->commands(i));
                switch(cmd->respond_id())
                {
                //法术牌凋零询问
                case 2451:
                    diaoLingFlag = true;
                    DiaoLing();
                    break;
                //非法术牌凋零询问
                case 2452:
                    diaoLingFlag = false;
                    DiaoLing();
                    break;
                }
            }
        }

        delete proto;
    }
    else
    {
        Role::decipher(proto_type, proto);
    }
}
