
#include "LingFu.h"




LingFu::LingFu()
{
    makeConnection();
    setMyRole(this);

    Button *leiMing,*fengXing,*checkCover;
    leiMing=new Button(3,QStringLiteral("灵符-雷鸣"));
    fengXing = new Button(4,QStringLiteral("灵符-风行"));

    buttonArea->addButton(leiMing);
    buttonArea->addButton(fengXing);
    //buttonArea->addButton(checkCover);
    connect(leiMing,SIGNAL(buttonSelected(int)),this,SLOT(leiMing()));
    connect(fengXing,SIGNAL(buttonSelected(int)),this,SLOT(fengXing()));

    checkCover = new Button(10,QStringLiteral("查看妖力"));
    buttonArea->addOutsideTurnButton(checkCover);

    checkCover->setVisible(true);
    checkCover->setEnabled(true);
    connect(checkCover,SIGNAL(buttonSelected(int)),gui,SLOT(showCoverArea()));
    connect(checkCover,SIGNAL(buttonUnselected(int)),gui,SLOT(closeCoverArea()));


}

void LingFu::normal()
{
    Role::normal();
    //灵符
    if (handArea->checkElement("thunder"))
        buttonArea->enable(3);
    if (handArea->checkElement("wind"))
        buttonArea->enable(4);

    unactionalCheck();
}



void LingFu::onOkClicked()
{
    Role::onOkClicked();

    QString command;
    QString sourceID;
    QString targetID;
    QString text,cardID;
    QList<Card*> selectedCards = handArea->getSelectedCards();
    QList<Card*> selectedCoverCards = coverArea->getSelectedCards();
    QList<Player*> selectedPlayers = playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;

    switch(state)
    {
    //念咒
    case 1801:
        respond = newRespond(1801);
        respond->add_args(selectedCards[0]->getID());

        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    //灵符
    case 1802:
    case 1803:
        action = newAction(state);
        action->add_args(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());
        action->add_dst_ids(selectedPlayers[1]->getID());

        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    //灵力崩解
    case 1804:
        respond = newRespond(1804);

        text=tipArea->getBoxCurrentText();
        if(text[0]=='1')
            respond->add_args(0);
        else
            respond->add_args(1);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    //百鬼夜行
    case 1805:
        respond = newRespond(1805);
        respond->add_args(selectedCoverCards[0]->getID());

        emit sendCommand(network::MSG_RESPOND, respond);

        dataInterface->removeCoverCard(selectedCoverCards[0]);
        coverArea->reset();
        gui->showCoverArea(false);
        gui->reset();
        break;
    //百鬼夜行-询问是否展示火妖力
    case 18050201:
        baiGuiYeXing3(true);
        break;
    //百鬼夜行-(火)选择目标
    case 180502:
        respond = newRespond(180502);
        respond->add_args(1);
        respond->add_dst_ids(selectedPlayers[0]->getID());
        if(selectedPlayers.count() > 1)
            respond->add_dst_ids(selectedPlayers[1]->getID());

        emit sendCommand(network::MSG_RESPOND, respond);
//        coverArea->removeCardItem(selectedCoverCards[0]);
//        coverArea->reset();
        gui->showCoverArea(false);
        gui->reset();
        break;
    //百鬼夜行-（普）选择目标
    case 180504:
        respond = newRespond(180504);
        respond->add_args(1);
        respond->add_dst_ids(selectedPlayers[0]->getID());

        emit sendCommand(network::MSG_RESPOND, respond);
        gui->showCoverArea(false);
        gui->reset();
        break;
    }
}

void LingFu::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;

    network::Respond* respond;

    switch(state)
    {    

    //念咒
    case 1801:
        respond = newRespond(1801);

        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 1:
    //灵符
    case 1802:
    case 1803:
        normal();
        break;
    //灵力崩解
    case 1804:
        respond = newRespond(1804);

        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    //百鬼夜行
    case 1805:
        respond = newRespond(1805);

        emit sendCommand(network::MSG_RESPOND, respond);
        coverArea->reset();
        gui->showCoverArea(false);
        gui->reset();
        break;
    //百鬼夜行-询问是否展示火妖力
    case 18050201:
        baiGuiYeXing3(false);
        break;

    }
}

void LingFu::leiMing()
{
    state=1802;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(2);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement(QStringLiteral("thunder"));
}

void LingFu::fengXing()
{
    state=1803;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(2);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement(QStringLiteral("wind"));
}

void LingFu::nianZhou()
{
    state=1801;
    tipArea->setMsg(QStringLiteral("是否发动念咒？"));
    handArea->setQuota(1);
    handArea->enableAll();
    decisionArea->enable(1);
}

void LingFu::lingLiBengJie()
{
    state=1804;
    tipArea->setMsg(QStringLiteral("是否发动灵力崩解？"));


    decisionArea->enable(0);

    if(dataInterface->getMyself()->getCrystal()>=1)
    {
        tipArea->addBoxItem(QStringLiteral("1.水晶"));
        decisionArea->enable(1);
    }
    if(dataInterface->getMyself()->getGem()>=1)
    {
        tipArea->addBoxItem(QStringLiteral("2.宝石"));
        decisionArea->enable(1);
    }
    tipArea->showBox();
}

void LingFu::baiGuiYeXing()
{
    state = 1805;
    tipArea->setMsg(QStringLiteral("是否发动百鬼夜行？要发动请选择妖力"));

    gui->showCoverArea(true);
    HandArea *coverArea = gui->getCoverArea();
    coverArea->reset();
    coverArea->enableAll();
    coverArea->setQuota(1);
    decisionArea->enable(1);
}

void LingFu::baiGuiYeXing2()
{
    state = 18050201;
    tipArea->setMsg(QStringLiteral("是否展示火属性妖力？"));

    decisionArea->enable(0);
    decisionArea->enable(1);
}

void LingFu::baiGuiYeXing3(bool show)
{
    state = 180502;
    if(show)
    {
        tipArea->setMsg("请选择两名角色不受伤害，其他角色各受1点法术伤害");
        playerArea->setQuota(2);
    }
    else
    {
        tipArea->setMsg("请选择1名角色，对其造成1点法术伤害");
        playerArea->setQuota(1);
    }

    playerArea->enableAll();

    decisionArea->reset();
}

void LingFu::baiGuiYeXing4()
{
    state = 180504;
    tipArea->setMsg("请选择1名角色，对其造成1点法术伤害");
    playerArea->setQuota(1);
    playerArea->enableAll();
    decisionArea->reset();
}

void LingFu::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==QStringLiteral("念咒"))
        nianZhou();
    else if(skill==QStringLiteral("灵力崩解"))
        lingLiBengJie();
    else if(skill == QStringLiteral("百鬼夜行"))
        baiGuiYeXing();
}

void LingFu::coverCardAnalyse()
{
    QList<Card*> selectedCoverCards = this->coverArea->getSelectedCards();
    switch(state)
    {
    //百鬼夜行
    case 1805:
        decisionArea->enable(0);
    }
}

void LingFu::cardAnalyse()
{
    Role::cardAnalyse();

    switch (state)
    {

//念咒
    case 1801:
        decisionArea->enable(0);
        break;
//风雷双符
    case 1802:
    case 1803:
        playerArea->enableAll();
        break;

    }
}

void LingFu::decipher(uint16_t proto_type, google::protobuf::Message* proto)
{
    if (proto_type == network::MSG_CMD_REQ)
    {
        Role::decipher(proto_type, proto);

        network::CommandRequest* cmd_req = (network::CommandRequest*)proto;
        if (cmd_req->cmd_type() == network::CMD_RESPOND)
        {
            for (int i = 0; i < cmd_req->commands_size(); ++i)
            {
                network::Command* cmd = (network::Command*)&(cmd_req->commands(i));
                switch(cmd->respond_id())
                {
    //百鬼夜行-火妖力-展示及目标询问
                case 180501:
                    this->baiGuiYeXing2();
                    break;
    //百鬼夜行-普通妖力-目标询问
                case 180503:
                    this->baiGuiYeXing4();
                    break;
                }
            }
        }
    }
    else
    {
        Role::decipher(proto_type, proto);
    }
}
