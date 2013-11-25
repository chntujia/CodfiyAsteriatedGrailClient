#include "ShenGuan.h"
enum CAUSE{
    SHEN_SHENG_QI_SHI = 1501,
    SHEN_SHENG_QI_FU = 1502,
    SHUI_ZHI_SHEN_LI = 1503,
    SHENG_SHI_SHOU_HU = 1504,
    SHEN_SHENG_QI_YUE = 1505,
    SHEN_SHENG_LING_YU = 1506,
    SHUI_ZHI_SHEN_LI_GIVE = 1531,
    SHUI_ZHI_SHEN_LI_CROSS = 1532,
    SHEN_SHENG_QI_YUE_1 = 1551,
    SHEN_SHENG_QI_YUE_2 = 1552,
    SHEN_SHENG_LING_YU_1 = 1561,
    SHEN_SHENG_LING_YU_2 = 1562
};
ShenGuan::ShenGuan()
{
    makeConnection();
    setMyRole(this);
    Button *shenShengQiFu, *shuiZhiShenLi, *shenShengLingYu;
    shenShengQiFu = new Button(3,QStringLiteral("神圣祈福"));
    buttonArea->addButton(shenShengQiFu);
    connect(shenShengQiFu,SIGNAL(buttonSelected(int)),this,SLOT(ShenShengQiFu()));

    shuiZhiShenLi = new Button(4,QStringLiteral("水之神力"));
    buttonArea->addButton(shuiZhiShenLi);
    connect(shuiZhiShenLi,SIGNAL(buttonSelected(int)),this,SLOT(ShuiZhiShenLi1()));

    shenShengLingYu = new Button(5,QStringLiteral("神圣领域"));
    buttonArea->addButton(shenShengLingYu);
    connect(shenShengLingYu,SIGNAL(buttonSelected(int)),this,SLOT(ShenShengLingYu1()));
}

void ShenGuan::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    QList<Card*> handcards=dataInterface->getHandCards();
    int qiFu = 0;
    for(int i=0; i<handcards.size();i++)
    {
        if(handcards[i]->getType() == QStringLiteral("magic"))
            qiFu++;
    }
    if(qiFu>1)
        buttonArea->enable(3);
    if(handArea->checkElement("water"))
        buttonArea->enable(4);
    if(myself->getEnergy()>0)
        buttonArea->enable(5);
    unactionalCheck();
}


void ShenGuan::ShenShengQiFu()
{
    state = SHEN_SHENG_QI_FU;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    handArea->setQuota(2);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableMagic();
}

void ShenGuan::ShuiZhiShenLi1()
{
    state = SHUI_ZHI_SHEN_LI;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("water");
}

void ShenGuan::ShuiZhiShenLi2()
{
    state = SHUI_ZHI_SHEN_LI_GIVE;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    tipArea->setMsg(QStringLiteral("请给目标角色一张牌"));

    handArea->setQuota(1);
    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableAll();
}

void ShenGuan::ShenShengQiYue1()
{
    state = SHEN_SHENG_QI_YUE_1;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动神圣契约"));
    QList<Card*> handcards=dataInterface->getHandCards();
    bool flag=true;
    if(handcards.size()==1 && handcards.at(0)->getType()=="light" && dataInterface->getMyself()->getEnergy()==1)
        flag=false;
    if(flag)
        decisionArea->enable(0);
    decisionArea->enable(1);
}

void ShenGuan::ShenShengQiYue2()
{
    state = SHEN_SHENG_QI_YUE_2;
    Player* myself=dataInterface->getMyself();
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    int cross = myself->getCrossNum();
    if (cross>4)
        cross = 4;
    for(;cross>0;cross--)
        tipArea->addBoxItem(QString::number(cross));
    tipArea->setMsg(QStringLiteral("请选择要转移的治疗数目"));
    tipArea->showBox();

    playerArea->setQuota(1);
    playerArea->enableMate();

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void ShenGuan::ShenShengLingYu1()
{
    state = SHEN_SHENG_LING_YU_1;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->enable(0);

    tipArea->setMsg(QStringLiteral("请先选择一项："));
    if(dataInterface->getMyself()->getCrossNum()>0)
        tipArea->addBoxItem(QStringLiteral("1.（移除1治疗）对目标角色造成2点法术伤害"));
    tipArea->addBoxItem(QStringLiteral("2.增加2治疗，目标队友增加1治疗"));
    tipArea->showBox();
}

void ShenGuan::ShenShengLingYu2()
{
    state = SHEN_SHENG_LING_YU_2;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);

    QList<Card*> handcards=dataInterface->getHandCards();
    if(handcards.size()>1)
        handArea->setQuota(2);
    else if(handcards.size()==1)
        handArea->setQuota(1);
    else
    {
        if(lingYu==1)
            playerArea->enableAll();
        else
            playerArea->enableMate();
    }

    handArea->enableAll();
    decisionArea->enable(1);
    decisionArea->disable(0);
}

void ShenGuan::cardAnalyse()
{
    Role::cardAnalyse();
    switch (state)
    {
    case SHEN_SHENG_QI_FU:
        decisionArea->enable(0);
        break;
    case SHUI_ZHI_SHEN_LI:
        playerArea->enableMate();
        break;
    case SHUI_ZHI_SHEN_LI_GIVE:
        decisionArea->enable(0);
        break;
    case SHEN_SHENG_LING_YU_2:
        if(lingYu==1)
            playerArea->enableAll();
        else
            playerArea->enableMate();
        break;
    }
}

void ShenGuan::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;

    switch(state)
    {
    case SHEN_SHENG_QI_FU:
        action = newAction(ACTION_MAGIC_SKILL, SHEN_SHENG_QI_FU);
        foreach(Card*ptr,selectedCards){
            action->add_card_ids(ptr->getID());
        }
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case SHUI_ZHI_SHEN_LI:
        action = newAction(ACTION_MAGIC_SKILL, SHUI_ZHI_SHEN_LI);
        action->add_card_ids(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case SHUI_ZHI_SHEN_LI_GIVE:
        respond = newRespond(SHUI_ZHI_SHEN_LI_GIVE);
        respond->add_card_ids(selectedCards[0]->getID());
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case SHEN_SHENG_QI_YUE_1:
        ShenShengQiYue2();
        break;
    case SHEN_SHENG_QI_YUE_2:
        respond = newRespond(SHEN_SHENG_QI_YUE);
        respond->add_dst_ids(selectedPlayers[0]->getID());
        respond->add_args(tipArea->getBoxCurrentText().toInt());
        start = true;
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case SHEN_SHENG_LING_YU_1:
        if(tipArea->getBoxCurrentText()[0]=='1')
            lingYu = 1;
        else
            lingYu = 2;
        ShenShengLingYu2();
        break;
    case SHEN_SHENG_LING_YU_2:
        action = newAction(ACTION_MAGIC_SKILL, SHEN_SHENG_LING_YU);
        action->add_args(lingYu);
        action->add_dst_ids(selectedPlayers[0]->getID());
        foreach(Card*ptr,selectedCards){
            action->add_card_ids(ptr->getID());
        }
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    }
}

void ShenGuan::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;

    network::Respond* respond;

    switch(state)
    {
    case SHEN_SHENG_QI_FU:
    case SHUI_ZHI_SHEN_LI:
    case SHEN_SHENG_LING_YU_1:
    case SHEN_SHENG_LING_YU_2:
        normal();
        break;
    case SHEN_SHENG_QI_YUE_1:
        respond = newRespond(SHEN_SHENG_QI_YUE);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case SHEN_SHENG_QI_YUE_2:
        respond = newRespond(SHEN_SHENG_QI_YUE);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
}

void ShenGuan::askForSkill(network::Command* cmd)
{
    switch (cmd->respond_id())
    {
    case SHUI_ZHI_SHEN_LI_GIVE:
        ShuiZhiShenLi2();
        break;
    case SHEN_SHENG_QI_YUE:
        ShenShengQiYue1();
        break;
    default:
        Role::askForSkill(cmd);
    }
}
