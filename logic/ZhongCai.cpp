#include "ZhongCai.h"

enum {
    YI_SHI_ZHONG_DUAN = 1401,
    ZHONG_CAI_YI_SHI = 1402,
    MO_RI_SHEN_PAN = 1403,
    SHEN_PAN_LANG_CHAO = 1404,
    PAN_JUE_TIAN_PING = 1405,
};

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
    //判决天平getElementCount
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
    state=YI_SHI_ZHONG_DUAN;
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
    state=ZHONG_CAI_YI_SHI;
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
    state=PAN_JUE_TIAN_PING;
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
    case YI_SHI_ZHONG_DUAN:
        respond = newRespond(YI_SHI_ZHONG_DUAN);
        respond->add_args(1);
        start=true;
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case ZHONG_CAI_YI_SHI:
        respond = newRespond(ZHONG_CAI_YI_SHI);
        respond->add_args(1);
        start=true;
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case MO_RI_SHEN_PAN:
        action = newAction(ACTION_MAGIC_SKILL, MO_RI_SHEN_PAN);
        action->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case PAN_JUE_TIAN_PING:
        action = newAction(ACTION_MAGIC_SKILL, PAN_JUE_TIAN_PING);
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1')
            action->add_args(0);
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
    case MO_RI_SHEN_PAN:
    //判决天平
    case PAN_JUE_TIAN_PING:
        normal();
        break;
    //仪式中断
    case YI_SHI_ZHONG_DUAN:
        respond = newRespond(YI_SHI_ZHONG_DUAN);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    //仲裁仪式
    case ZHONG_CAI_YI_SHI:
        respond = newRespond(ZHONG_CAI_YI_SHI);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
}
void ZhongCai::askForSkill(network::Command* cmd)
{
    if(cmd->respond_id() == YI_SHI_ZHONG_DUAN)
        YiShiZhongDuan();
    else if(cmd->respond_id() == ZHONG_CAI_YI_SHI)
        ZhongCaiYiShi();
    else
        Role::askForSkill(cmd);
}
