#include "JianDi.h"

JianDi::JianDi()
{
    makeConnection();
    setMyRole(this);

    Button *checkCover;
    checkCover = new Button(10,QStringLiteral("查看剑魂"));
    buttonArea->addOutsideTurnButton(checkCover);

    checkCover->setVisible(true);
    checkCover->setEnabled(true);
    connect(checkCover,SIGNAL(buttonSelected(int)),gui,SLOT(showCoverArea()));
    connect(checkCover,SIGNAL(buttonUnselected(int)),gui,SLOT(closeCoverArea()));
}

void JianDi::JianHunShouHu(int cardID)
{
    state=1901;
    tipArea->setMsg(QStringLiteral("是否发动剑魂守护？"));
    shouhuID=cardID;
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void JianDi::JianQiZhan(int targetID)
{
    Player* myself=dataInterface->getMyself();
    state=1902;
    tipArea->setMsg(QStringLiteral("请选择要移除的剑气数："));
    int min=myself->getToken(0)<3?myself->getToken(0):3;
    for(;min>0;min--)
        tipArea->addBoxItem(QString::number(min));
    tipArea->showBox();
    jianqiID=targetID;
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void JianDi::JianQiZhan2()
{
    state=1952;
    tipArea->reset();
    decisionArea->reset();
    playerArea->reset();

    playerArea->enableAll();
    playerArea->disablePlayerItem(jianqiID);
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void JianDi::TianShiZhiHun()
{
    state=1903;
    tipArea->setMsg(QStringLiteral("是否发动天使之魂？如发动请选择剑魂："));
    decisionArea->enable(1);
    decisionArea->disable(0);
    gui->showCoverArea(true);
    HandArea *coverArea = gui->getCoverArea();
    coverArea->reset();
    coverArea->enableAll();
    coverArea->setQuota(1);
}

void JianDi::EMoZhiHun()
{
    state=1904;
    tipArea->setMsg(QStringLiteral("是否发动恶魔之魂？如发动请选择剑魂："));
    decisionArea->enable(1);
    decisionArea->disable(0);
    gui->showCoverArea(true);
    HandArea *coverArea = gui->getCoverArea();
    coverArea->reset();
    coverArea->enableAll();
    coverArea->setQuota(1);
}

void JianDi::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==QStringLiteral("剑魂守护"))
        JianHunShouHu(command.split(';').at(3).toInt());
    else if(skill==QStringLiteral("剑气斩"))
        JianQiZhan(command.split(';').at(3).toInt());
    else if(skill==QStringLiteral("天使之魂"))
        TianShiZhiHun();
    else if(skill==QStringLiteral("恶魔之魂"))
        EMoZhiHun();

}

void JianDi::additionalAction()
{
    Role::additionalAction();
    if(usedAttack && dataInterface->getMyself()->getEnergy()>0)
        tipArea->addBoxItem(QStringLiteral("1.不屈意志"));
}

void JianDi::coverCardAnalyse()
{
    switch(state)
    {
    case 1903:
    case 1904:
        decisionArea->enable(0);
        break;
    }
}

void JianDi::onOkClicked()
{
    Role::onOkClicked();

    QString command;
    QString sourceID;
    QString targetID;
    QString text,cardID;
    QList<Card*> selectedCards = handArea->getSelectedCards();
    QList<Card*> selectedCoverCards = coverArea->getSelectedCards();
    QList<Player*> selectedPlayers = playerArea->getSelectedPlayers();

    network::Respond* respond;

    switch(state)
    {
    case 42:
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1'){
            respond = new network::Respond();
            respond->set_src_id(myID);
            respond->set_respond_id(1906);
            respond->add_args(1);

            emit sendCommand(network::MSG_RESPOND, respond);
            attackAction();
        }
        break;
    case 1901:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(1901);
        respond->add_args(1);

        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 1902:
        jianqiPoint=tipArea->getBoxCurrentText();
        JianQiZhan2();        
        break;
    case 1952:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(1902);
        respond->add_dst_ids(selectedPlayers[0]->getID());
        respond->add_args(jianqiPoint.toInt());

        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 1903:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(1903);
        respond->add_args(selectedCoverCards[0]->getID());

        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 1904:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(1904);
        respond->add_args(selectedCoverCards[0]->getID());

        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
    gui->showCoverArea(false);
}

void JianDi::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;

    network::Respond* respond;

    switch(state)
    {
    case 1901:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(1901);

        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 1902:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(1902);

        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 1952:
        JianQiZhan(jianqiID);
        break;
    case 1903:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(1903);

        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 1904:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(1904);

        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
    gui->showCoverArea(false);
}
