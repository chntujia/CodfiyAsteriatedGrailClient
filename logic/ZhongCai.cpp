#include "ZhongCai.h"

ZhongCai::ZhongCai()
{
    makeConnection();
    setMyRole(this);

    Button *moRiShenPan;
    moRiShenPan=new Button(3,QStringLiteral("末日审判"));
    buttonArea->addButton(moRiShenPan);
    connect(moRiShenPan,SIGNAL(buttonSelected(int)),this,SLOT(MoRiShenPan()));

    Button *panJueTianPing;
    panJueTianPing=new Button(4,QStringLiteral("判决天平"));
    buttonArea->addButton(panJueTianPing);
    connect(panJueTianPing,SIGNAL(buttonSelected(int)),this,SLOT(PanJueTianPing()));
}

void ZhongCai::normal()
{
    Role::normal();
    Player *myself=dataInterface->getMyself();
    //末日审判
    if(myself->getToken(0)>0)
        buttonArea->enable(3);
    //判决天平
    if(myself->getEnergy()>0)
        buttonArea->enable(4);
    //强制末日
    if(myself->getToken(0)==4)
    {
        handArea->disableAll();
        buttonArea->disable(0);
        buttonArea->disable(1);
        buttonArea->disable(2);
        buttonArea->disable(4);
    }
    unactionalCheck();
}

void ZhongCai::YiShiZhongDuan()
{
    state=1401;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动仪式中断？"));
    QList<Card*> handcards=dataInterface->getHandCards();
    Player *myself=dataInterface->getMyself();
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
        if(myself->getToken(0)>0 || myself->getEnergy()>0)
            flag=true;
    }
    if(flag)
        decisionArea->enable(0);
}

void ZhongCai::ZhongCaiYiShi()
{
    state=1402;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动仲裁仪式？"));
    QList<Card*> handcards=dataInterface->getHandCards();
    Player *myself=dataInterface->getMyself();
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
        if(myself->getToken(0)>0 || myself->getEnergy()>0)
            flag=true;
    }
    if(flag)
        decisionArea->enable(0);
}

void ZhongCai::MoRiShenPan()
{
    state=1403;
    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    playerArea->enableAll();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void ZhongCai::PanJueTianPing()
{
    state=1404;
    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    decisionArea->enable(0);
    decisionArea->enable(1);

    tipArea->setMsg(QStringLiteral("请选择一项："));
    tipArea->addBoxItem(QStringLiteral("1.弃掉你的所有手牌"));
    tipArea->addBoxItem(QStringLiteral("2.将手牌补到上限，【战绩区】+1【宝石】"));
    tipArea->showBox();
}

void ZhongCai::onOkClicked()
{
    Role::onOkClicked();
    QList<Player*>selectedPlayers;

    QString command;
    QString sourceID;
    QString targetID;
    QString text;

    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;

    switch(state)
    {
    case 1401:
        respond = newRespond(1401);
        respond->add_args(1);
        start=true;
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 1402:
        respond = newRespond(1402);
        respond->add_args(1);
        start=true;
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case 1403:
        action = newAction(1403);
        action->add_args(1);
        action->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case 1404:
        action = newAction(1404);
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1'){
            action->add_args(0);
            foreach(Card*ptr,dataInterface->getHandCards())
                dataInterface->removeHandCard(ptr);
        }
        else
            action->add_args(1);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    }
}

void ZhongCai::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;

    network::Respond* respond;
    switch(state)
    {
    //末日审判
    case 1403:
    //判决天平
    case 1404:
        normal();
        break;
    //仪式中断
    case 1401:
        respond = newRespond(1401);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    //仲裁仪式
    case 1402:
        respond = newRespond(1402);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
}
void ZhongCai::askForSkill(QString skill)
{
    //Role::askForSkill(skill);
    if(skill==QStringLiteral("仪式中断"))
        YiShiZhongDuan();
    else if(skill==QStringLiteral("仲裁仪式"))
        ZhongCaiYiShi();
}
