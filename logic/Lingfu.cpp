#include "LingFu.h"

enum CAUSE{
    FENG_XING = 1801,
    LEI_MING = 1802,
    NIAN_ZHOU = 1803,
    BAI_GUI_YE_XING = 1804,
    LING_LI_BENG_JIE = 1805,
    BAI_GUI_YE_XING_EXPOSE = 1806,
    BAI_GUI_YE_XING_FIRE = 1807
};

LingFu::LingFu()
{
    makeConnection();
    setMyRole(this);

    Button *leiMing,*fengXing,*checkCover;
    leiMing = new Button(3, QStringLiteral("灵符-雷鸣"));
    fengXing = new Button(4, QStringLiteral("灵符-风行"));

    buttonArea->addButton(leiMing);
    buttonArea->addButton(fengXing);

    connect(leiMing, SIGNAL(buttonSelected(int)), this, SLOT(LeiMing()));
    connect(fengXing, SIGNAL(buttonSelected(int)), this, SLOT(FengXing()));

    checkCover = new Button(10, QStringLiteral("查看妖力"));
    buttonArea->addOutsideTurnButton(checkCover);

    checkCover->setVisible(true);
    checkCover->setEnabled(true);
    connect(checkCover, SIGNAL(buttonSelected(int)), gui, SLOT(showCoverArea()));
    connect(checkCover, SIGNAL(buttonUnselected(int)), gui, SLOT(closeCoverArea()));
}

void LingFu::normal()
{
    Role::normal();
    //灵符
    if (handArea->checkElement("thunder")){
        buttonArea->enable(3);
    }
    if (handArea->checkElement("wind")){
        buttonArea->enable(4);
    }
    unactionalCheck();
}

void LingFu::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*> selectedCards = handArea->getSelectedCards();
    QList<Player*> selectedPlayers = playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;

    switch(state)
    {
    //灵符
    case LEI_MING:
    case FENG_XING:
        action = newAction(ACTION_MAGIC_SKILL, state);
        action->add_card_ids(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());
        action->add_dst_ids(selectedPlayers[1]->getID());

        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    //百鬼夜行-询问是否展示火妖力
    case BAI_GUI_YE_XING_EXPOSE:
        gui->reset();
        BaiGuiYeXing_Fire();
        break;
    //百鬼夜行-(火)选择目标
    case BAI_GUI_YE_XING_FIRE:
        respond = newRespond(BAI_GUI_YE_XING);
        respond->add_args(1);
        respond->add_dst_ids(selectedPlayers[0]->getID());
        respond->add_dst_ids(selectedPlayers[1]->getID());
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    //百鬼夜行-（普）选择目标
    case BAI_GUI_YE_XING:
        respond = newRespond(BAI_GUI_YE_XING);
        respond->add_args(0);
        respond->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
}

void LingFu::onCancelClicked()
{
    Role::onCancelClicked();
    network::Respond* respond;

    switch(state)
    {    
    //灵符
    case LEI_MING:
    case FENG_XING:
        normal();
        break;
    //灵力崩解
    case LING_LI_BENG_JIE:
        respond = newRespond(LING_LI_BENG_JIE);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    //百鬼夜行-询问是否展示火妖力
    case BAI_GUI_YE_XING_EXPOSE:
        BaiGuiYeXing(true);
        break;
    //百鬼夜行-(火)选择目标
    case BAI_GUI_YE_XING_FIRE:
        BaiGuiYeXing_Expose();
        break;
    //百鬼夜行-（普）选择目标
    case BAI_GUI_YE_XING:
        BaiGuiYeXing_Expose();
        break;
    }
}

void LingFu::LeiMing()
{
    state = LEI_MING;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(2);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("thunder");
}

void LingFu::FengXing()
{
    state = FENG_XING;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(2);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("wind");
}

void LingFu::BaiGuiYeXing_Expose()
{
    state = BAI_GUI_YE_XING_EXPOSE;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否展示火属性妖力？"));

    decisionArea->enable(0);
    decisionArea->enable(1);
}

void LingFu::BaiGuiYeXing_Fire()
{
    state = BAI_GUI_YE_XING_FIRE;
    gui->reset();
    tipArea->setMsg(QStringLiteral("请选择两名角色不受伤害，其他角色各受1点法术伤害"));
    playerArea->setQuota(2);
    playerArea->enableAll();
    decisionArea->enable(1);
}

void LingFu::BaiGuiYeXing(bool canRevert)
{
    state = BAI_GUI_YE_XING;
    gui->reset();
    tipArea->setMsg(QStringLiteral("请选择1名角色，对其造成1点法术伤害"));
    playerArea->setQuota(1);
    playerArea->enableAll();
    if(canRevert){
        decisionArea->enable(1);
    }
}

void LingFu::askForSkill(Command *cmd)
{
    if(cmd->respond_id() == BAI_GUI_YE_XING){
        if(cmd->args(0) == 1){
            BaiGuiYeXing_Expose();
        }
        else{
            BaiGuiYeXing(false);
        }
    }
    else{
        Role::askForSkill(cmd);
    }
}

void LingFu::drop(int howMany, int cause)
{
    Role::drop(howMany, cause);
    if(cause == NIAN_ZHOU){
        tipArea->setMsg(QStringLiteral("是否发动念咒？"));
        decisionArea->enable(1);
    }
}

void LingFu::dropCover(int howMany, int cause)
{
    Role::dropCover(howMany, cause);
    if(cause == BAI_GUI_YE_XING){
        tipArea->setMsg(QStringLiteral("是否发动百鬼夜行？"));
        decisionArea->enable(1);
    }
}

void LingFu::cardAnalyse()
{
    Role::cardAnalyse();

    switch (state)
    {
//风雷双符
    case LEI_MING:
    case FENG_XING:
        playerArea->enableAll();
        break;

    }
}
