#include "ShiRen.h"
enum CAUSE{
    CHEN_LUN_XIE_ZOU_QU=3101,
    BU_XIE_HE_XIAN=3102,
    BU_XIE_HE_XIAN_TARGET=31021,
    GE_YONG_TIAN_FU=3103,
    BAO_FENG_QIAN_ZOU_QU=3104,
    JI_ANG_KUANG_XIANG_QU=3105,
    JI_ANG_KUANG_XIANG_QU_STONE=31051,
    JI_ANG_KUANG_XIANG_QU_2=31052,
    JI_ANG_KUANG_XIANG_QU_HARM=31053,
    SHENG_LI_JIAO_XIANG_SHI=3106,
    SHENG_LI_JIAO_XIANG_SHI_2=31061,
    SHENG_LI_JIAO_XIANG_SHI_STONE=31062,
    XI_WANG_FU_GE_QU=3107
};
ShiRen::ShiRen()
{
    YueZhangDst = dataInterface->getMyself()->getID();
    makeConnection();
    setMyRole(this);
    Button *buXieHeXian;
    buXieHeXian = new Button(3,QStringLiteral("不谐和弦"));
    buttonArea->addButton(buXieHeXian);
    connect(buXieHeXian,SIGNAL(buttonSelected(int)),this,SLOT(BuXieHeXian()));
}

void ShiRen::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    if(myself->getToken(0)>1)
        buttonArea->enable(3);
    unactionalCheck();
}

void ShiRen::BuXieHeXian()
{
    state = BU_XIE_HE_XIAN;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    Player* myself=dataInterface->getMyself();
    for(int i=2; i<=myself->getToken(0); i++)
    {
        tipArea->addBoxItem(QString::number(i));
    }
    tipArea->setMsg(QStringLiteral("选择移除灵感数量"));
    tipArea->showBox();

    decisionArea->enable(1);
    decisionArea->enable(0);
}

void ShiRen::BuXieHeXianTarget()
{
    state = BU_XIE_HE_XIAN_TARGET;

    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    tipArea->addBoxItem(QStringLiteral("1.你和目标角色各摸（x-1）张牌【强制】"));
    tipArea->addBoxItem(QStringLiteral("2.你和目标角色各弃x张牌【强制】"));
    tipArea->setMsg(QStringLiteral("选择技能效果"));
    tipArea->showBox();

    playerArea->enableAll();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void ShiRen::ChenLunXieZouQu()
{
    state = CHEN_LUN_XIE_ZOU_QU;

    gui->reset();

    tipArea->setMsg(QStringLiteral("选择【沉沦协奏曲】伤害目标或取消"));

    handArea->enableAll();
    handArea->setQuota(2);

    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void ShiRen::XiWangFuGeQu(int id)
{
    state = XI_WANG_FU_GE_QU;

    gui->reset();

    tipArea->setMsg(QStringLiteral("选择【希望赋格曲】目标或取消"));

    playerArea->enableMate(true);
    playerArea->disablePlayerItem(id);
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void ShiRen::onOkClicked()
{
    Role::onOkClicked();
    SafeList<Card*> selectedCards;
    SafeList<Player*>selectedPlayers;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;
    try{
    switch(state)
    {
    case CHEN_LUN_XIE_ZOU_QU:
        respond = newRespond(CHEN_LUN_XIE_ZOU_QU);
        respond->add_args(1);
        respond->add_dst_ids(selectedPlayers[0]->getID());
        respond->add_card_ids(selectedCards[0]->getID());
        respond->add_card_ids(selectedCards[1]->getID());
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case XI_WANG_FU_GE_QU:
        respond = newRespond(XI_WANG_FU_GE_QU);
        respond->add_args(1);
        respond->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        start = true;
        break;

    case BU_XIE_HE_XIAN:
        chenLunNum = tipArea->getBoxCurrentText()[0].digitValue();
        BuXieHeXianTarget();
        break;
    case BU_XIE_HE_XIAN_TARGET:
        action = newAction(ACTION_MAGIC_SKILL, BU_XIE_HE_XIAN);
        action->add_dst_ids(selectedPlayers[0]->getID());
        action->add_args(chenLunNum);
        action->add_args(tipArea->getBoxCurrentText()[0].digitValue());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    }

    }catch(int error){
        logic->onError(error);
    }
}

void ShiRen::onCancelClicked()
{
    Role::onCancelClicked();

    network::Respond* respond;

    switch(state)
    {
    case CHEN_LUN_XIE_ZOU_QU:
        respond = newRespond(CHEN_LUN_XIE_ZOU_QU);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case XI_WANG_FU_GE_QU:
        respond = newRespond(XI_WANG_FU_GE_QU);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case BU_XIE_HE_XIAN:
    case BU_XIE_HE_XIAN_TARGET:
        normal();
        break;
    }
}

void ShiRen::askForSkill(network::Command* cmd)
{
    switch (cmd->respond_id())
    {
    case CHEN_LUN_XIE_ZOU_QU:
        ChenLunXieZouQu();
        break;
    case XI_WANG_FU_GE_QU:
        XiWangFuGeQu(cmd->args(0));
        break;
    default:
        Role::askForSkill(cmd);
    }
}

void ShiRen::cardAnalyse()
{
    SafeList<Card*> selectedCards;
    Role::cardAnalyse();
    switch (state)
    {
    case CHEN_LUN_XIE_ZOU_QU:
        selectedCards = handArea->getSelectedCards();
        if(selectedCards.size()==2 && (selectedCards[0]->getElement() == selectedCards[1]->getElement()))
            playerArea->enableEnemy();
        else
            playerArea->disableAll();
        break;
    }
}
