#include "TianShi.h"

enum {
    TIAN_SHI_ZHI_QIANG = 701,
    TIAN_SHI_ZHU_FU = 702,
    FENG_ZHI_JIE_JING = 703,
    TIAN_SHI_JI_BAN = 704,
    TIAN_SHI_ZHI_GE = 705,
    SHEN_ZHI_BI_HU = 706,
    TIAN_SHI_ZHI_GE_2 = 707,
};

TianShi::TianShi()
{
    makeConnection();
    setMyRole(this);

    connect(playerArea,SIGNAL(playerUnready()),this,SLOT(onUnready()));
    connect(handArea,SIGNAL(cardUnready()),this,SLOT(onUnready()));

    Button *fengZhiJieJin,*tianShiZhuFu,*tianShiZhiQiang;
    fengZhiJieJin=new Button(3,QStringLiteral("风之洁净"));
    buttonArea->addButton(fengZhiJieJin);
    connect(fengZhiJieJin,SIGNAL(buttonSelected(int)),this,SLOT(FengZhiJieJin()));

    tianShiZhuFu=new Button(4,QStringLiteral("天使祝福"));
    buttonArea->addButton(tianShiZhuFu);
    connect(tianShiZhuFu,SIGNAL(buttonSelected(int)),this,SLOT(TianShiZhuFu()));

    tianShiZhiQiang=new Button(5,QStringLiteral("天使之墙"));
    buttonArea->addButton(tianShiZhiQiang);
    connect(tianShiZhiQiang,SIGNAL(buttonSelected(int)),this,SLOT(TianShiZhiQiang()));
}
void TianShi::TianShiJiBan()
{
    playerArea->reset();
    tipArea->reset();

    state=TIAN_SHI_JI_BAN;
    tipArea->setMsg(QStringLiteral("发动天使羁绊"));
    playerArea->setQuota(1);
    playerArea->enableAll();

    decisionArea->disable(1);
    decisionArea->disable(0);
}

void TianShi::FengZhiJieJin()
{
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    state=FENG_ZHI_JIE_JING;
    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("wind");
}

void TianShi::TianShiZhuFu()
{
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    state=TIAN_SHI_ZHU_FU;
    playerArea->setQuota(1,2);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("water");
}
void TianShi::TianShiZhiQiang()
{
    state=TIAN_SHI_ZHI_QIANG;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableSpecility(QStringLiteral("天使之墙"));
}

void TianShi::TianShiZhiGe1()
{
    gui->reset();
    int gem,crystal;
    Player*myself=dataInterface->getMyself();

    gem=myself->getGem();
    crystal=myself->getCrystal();
    state=TIAN_SHI_ZHI_GE;
    tipArea->setMsg(QStringLiteral("是否发动天使之歌？"));
    if(crystal>=1)
        tipArea->addBoxItem(QStringLiteral("1.水晶"));
    if(gem>=1)
        tipArea->addBoxItem(QStringLiteral("2.宝石"));

    tipArea->showBox();
    decisionArea->enable(0);
    decisionArea->enable(1);
}
void TianShi::TianShiZhiGe2()
{
    SafeList<Player*> players=dataInterface->getPlayerList();
    int i;
    state=TIAN_SHI_ZHI_GE_2;

    for(i=0;i<players.size();i++)
    {
        if(players[i]->hasStatus())
            playerArea->enablePlayerItem(i);
    }
    playerArea->setQuota(1);
    decisionArea->enable(1);
}

void TianShi::ShenZhiBiHu(int reduce)
{
    int gem,crystal;
    Player* myself=dataInterface->getMyself();

    gem=myself->getGem();
    crystal=myself->getCrystal();

    state=SHEN_ZHI_BI_HU;

    tipArea->reset();
    handArea->reset();
    playerArea->reset();
    decisionArea->enable(0);
    decisionArea->enable(1);
    tipArea->setMsg(QStringLiteral("是否发动神之庇护？"));
    if(reduce>=3)
    {
        if(gem==3)
            tipArea->addBoxItem(QStringLiteral("1.三个宝石"));
        if(crystal==3)
            tipArea->addBoxItem(QStringLiteral("2.三个水晶"));
        if(gem==2&&crystal==1)
            tipArea->addBoxItem(QStringLiteral("3.两个宝石和一个水晶"));
        if(gem==1&&crystal==2)
            tipArea->addBoxItem(QStringLiteral("4.一个宝石和两个水晶"));
    }
    if(reduce>=2)
    {
        if(gem>=2)
            tipArea->addBoxItem(QStringLiteral("5.两个宝石"));
        if(crystal>=2)
            tipArea->addBoxItem(QStringLiteral("6.两个水晶"));
        if(gem>=1&&crystal>=1)
            tipArea->addBoxItem(QStringLiteral("7.一个宝石和一个水晶"));
    }
    if(gem>=1)
        tipArea->addBoxItem(QStringLiteral("8.一个宝石"));
    if(crystal>=1)
        tipArea->addBoxItem(QStringLiteral("9.一个水晶"));
    tipArea->showBox();
}

void TianShi::normal()
{
    Role::normal();
//风之洁净
    if (handArea->checkElement("wind"))
        foreach(Player*ptr,dataInterface->getPlayerList())
            if(ptr->hasStatus()){
                buttonArea->enable(3);
                break;
            }
//天使祝福
    if (handArea->checkElement("water"))
        buttonArea->enable(4);
//天使之墙
    if (handArea->checkSpecility(QStringLiteral("天使之墙")))
        buttonArea->enable(5);
    unactionalCheck();
}
void TianShi::playerAnalyse()
{
    switch (state)
    {
    case FENG_ZHI_JIE_JING:
    case TIAN_SHI_ZHI_GE_2:
        tipArea->showStatus(playerArea->getSelectedPlayers().at(0)->getID());
        break;
    default:
        decisionArea->enable(0);
        break;
    }

}

void TianShi::cardAnalyse()
{
    Role::cardAnalyse();

    SafeList<Player*>players=dataInterface->getPlayerList();
    int i;
    try{
    switch (state)
    {
    case FENG_ZHI_JIE_JING:
        for(i=0;i<players.size();i++)
            if(players[i]->hasStatus())
                playerArea->enablePlayerItem(i);
        break;
    case TIAN_SHI_ZHU_FU:
        playerArea->enableAll();
        break;
    case TIAN_SHI_ZHI_QIANG:
        for(i=0;i<dataInterface->getPlayerMax();i++)
            if(!dataInterface->getPlayerList().at(i)->checkBasicStatus(2))
                playerArea->enablePlayerItem(i);
        break;
    }

    }catch(int error){
        logic->onError(error);
    }
}

void TianShi::onOkClicked()
{
    Role::onOkClicked();
    SafeList<Card*> selectedCards;
    SafeList<Player*>selectedPlayers;

    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;
    try{
    switch(state)
    {
//风之洁净
    case FENG_ZHI_JIE_JING:
        action = newAction(ACTION_MAGIC_SKILL, FENG_ZHI_JIE_JING);
        action->add_card_ids(selectedCards[0]->getID());
        action->add_args(tipArea->getSelectedCardID());
        action->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//天使祝福
    case TIAN_SHI_ZHU_FU:
        action = newAction(ACTION_MAGIC_SKILL, TIAN_SHI_ZHU_FU);
        action->add_card_ids(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());
        if(selectedPlayers.size()>1)
            action->add_dst_ids(selectedPlayers[1]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//天使之墙
    case TIAN_SHI_ZHI_QIANG:
        action = newAction(ACTION_MAGIC_SKILL, TIAN_SHI_ZHI_QIANG);
        action->add_card_ids(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//天使之歌1
    case TIAN_SHI_ZHI_GE:
        text=tipArea->getBoxCurrentText();
        switch(text[0].digitValue())
        {
        case 1:
            usedGem=0;
            break;
        case 2:
            usedGem=1;
            break;
        }
        gui->reset();
        TianShiZhiGe2();
        break;
//神之庇护
        /*
            tipArea->addBoxItem(QStringLiteral("1.三个宝石"));
            tipArea->addBoxItem(QStringLiteral("2.三个水晶"));
            tipArea->addBoxItem(QStringLiteral("3.两个宝石和一个水晶"));
            tipArea->addBoxItem(QStringLiteral("4.一个宝石和两个水晶"));
            tipArea->addBoxItem(QStringLiteral("5.两个宝石"));
            tipArea->addBoxItem(QStringLiteral("6.两个水晶"));
            tipArea->addBoxItem(QStringLiteral("7.一个宝石和一个水晶"));
            tipArea->addBoxItem(QStringLiteral("8.一个宝石"));
            tipArea->addBoxItem(QStringLiteral("9.一个水晶"));*/
    case SHEN_ZHI_BI_HU:
        respond = newRespond(SHEN_ZHI_BI_HU);
        text=tipArea->getBoxCurrentText();
        switch(text[0].digitValue())
        {
        case 1:
            respond->add_args(3);
            respond->add_args(0);
            break;
        case 2:
            respond->add_args(0);
            respond->add_args(3);
            break;
        case 3:
            respond->add_args(2);
            respond->add_args(1);
            break;
        case 4:
            respond->add_args(1);
            respond->add_args(2);
            break;
        case 5:
            respond->add_args(2);
            respond->add_args(0);
            break;
        case 6:
            respond->add_args(0);
            respond->add_args(2);
            break;
        case 7:
            respond->add_args(1);
            respond->add_args(1);
            break;
        case 8:
            respond->add_args(1);
            respond->add_args(0);
            break;
        case 9:
            respond->add_args(0);
            respond->add_args(1);
            break;
        }
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//天使之歌2
    case TIAN_SHI_ZHI_GE_2:
        respond = newRespond(TIAN_SHI_ZHI_GE);
        if(usedGem)
            respond->add_args(1);
        else
            respond->add_args(0);
        respond->add_args(tipArea->getSelectedCardID());
        respond->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//天使羁绊
    case TIAN_SHI_JI_BAN:
        respond = newRespond(TIAN_SHI_JI_BAN);
        respond->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }

    }catch(int error){
        logic->onError(error);
    }
}

void TianShi::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;

    network::Respond* respond;
    switch(state)
    {
//风之洁净
    case FENG_ZHI_JIE_JING:
//天使祝福
    case TIAN_SHI_ZHU_FU:
//天使之墙
    case TIAN_SHI_ZHI_QIANG:
        gui->reset();
        normal();
        break;
//天使之歌1
    case TIAN_SHI_ZHI_GE:
        respond = newRespond(TIAN_SHI_ZHI_GE);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//神之庇护
    case SHEN_ZHI_BI_HU:
        respond = newRespond(SHEN_ZHI_BI_HU);
        respond->add_args(0);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//天使之歌2
    case TIAN_SHI_ZHI_GE_2:
        gui->reset();
        TianShiZhiGe1();
        break;
    }
}
void TianShi::onUnready()
{
    switch(state)
    {
    case FENG_ZHI_JIE_JING:
        tipArea->reset();
        FengZhiJieJin();
        break;
    case TIAN_SHI_ZHI_GE_2:
        tipArea->reset();
        TianShiZhiGe2();
        break;
    }
}
void TianShi::askForSkill(network::Command* cmd)
{
    if(cmd->respond_id() == TIAN_SHI_ZHI_GE)
        TianShiZhiGe1();
    else if(cmd->respond_id() == TIAN_SHI_JI_BAN)
        TianShiJiBan();
    else if(cmd->respond_id() == SHEN_ZHI_BI_HU)
        ShenZhiBiHu(cmd->args(0));
    else
        Role::askForSkill(cmd);
}
