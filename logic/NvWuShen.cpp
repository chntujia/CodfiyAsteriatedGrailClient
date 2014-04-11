#include "NvWuShen.h"

enum CAUSE{
    SHEN_SHENG_ZHUI_JI_AFTER_ATTACK = 2501,
    SHEN_SHENG_ZHUI_JI_AFTER_MAGIC = 25011,
    ZHI_XU_ZHI_YIN = 2502,
    HE_PING_XING_ZHE = 2503,
    JUN_SHEN_WEI_GUANG = 2504,
    JUN_SHEN_WEI_GUANG_2 = 25042,
    YING_LING_ZHAO_HUAN = 2505
};

NvWuShen::NvWuShen()
{
    makeConnection();
    setMyRole(this);
    Button *zhiXuZhiYin;
    zhiXuZhiYin = new Button(3,QStringLiteral("秩序之印"));
    buttonArea->addButton(zhiXuZhiYin);
    connect(zhiXuZhiYin,SIGNAL(buttonSelected(int)),this,SLOT(ZhiXuZhiYin()));
}

void NvWuShen::normal()
{
    Role::normal();
    buttonArea->enable(3);
    unactionalCheck();
}

void NvWuShen::JunShenWeiGuang()
{
    state =JUN_SHEN_WEI_GUANG;
    Team* myTeam=dataInterface->getMyTeam();
    tipArea->setMsg(QStringLiteral("选择军神威光效果"));
    tipArea->addBoxItem(QStringLiteral("1.你+1【治疗】"));
    if(myTeam->getEnergy()>1) {
        tipArea->addBoxItem(QStringLiteral("2.移除已方战绩区2星石无视上限+2治疗"));
    }
    tipArea->showBox();
    decisionArea->enable(0);
    decisionArea->disable(1);
}

void NvWuShen::JunShenWeiGuang2()
{
    state = JUN_SHEN_WEI_GUANG_2;
    gui->reset();
    Team* myTeam = dataInterface->getMyTeam();
    int gem, crystal;
    gem = myTeam->getGem();
    crystal = myTeam->getCrystal();
    tipArea->setMsg(QStringLiteral("请选择要移除的星石"));
    if(crystal>=2)
        tipArea->addBoxItem(QStringLiteral("1.两个水晶"));
    if(gem>=1&&crystal>=1)
        tipArea->addBoxItem(QStringLiteral("2.一个宝石和一个水晶"));
    if(gem>=2)
        tipArea->addBoxItem(QStringLiteral("3.两个宝石"));
    tipArea->showBox();

    decisionArea->enable(1);
    decisionArea->enable(0);
}

void NvWuShen::ZhiXuZhiYin()
{
    state = ZHI_XU_ZHI_YIN;
    gui->reset();

    decisionArea->enable(0);
    decisionArea->enable(1);
}

void NvWuShen::YingLingZhaoHuan()
{
    state = YING_LING_ZHAO_HUAN;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动英灵召唤？（若你额外弃1张法术牌【展示】）本次攻击伤害额外+1"));
    handArea->enableMagic();
    handArea->setQuota(0, 1);
    decisionArea->enable(1);
    decisionArea->enable(0);
}

void NvWuShen::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;


    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;
    int choice;
    int crystal, gem;
    switch(state)
    {
    case JUN_SHEN_WEI_GUANG:
        choice = tipArea->getBoxCurrentText()[0].digitValue();
        if(choice == 1) {
            respond = newRespond(JUN_SHEN_WEI_GUANG);
            respond->add_args(1);
            emit sendCommand(network::MSG_RESPOND, respond);
            gui->reset();
        }
        else {
            gui->reset();
            JunShenWeiGuang2();
        }
        break;
    case JUN_SHEN_WEI_GUANG_2:
        respond = newRespond(JUN_SHEN_WEI_GUANG);
        respond->add_args(2);
        choice = tipArea->getBoxCurrentText()[0].digitValue();
        if(choice == 1) {
            crystal = 2;
            gem = 0;
        } else
        if(choice == 2) {
            crystal = 1;
            gem = 1;
        } else
        if(choice == 3) {
            crystal = 0;
            gem = 2;
        }
        respond->add_args(gem);
        respond->add_args(crystal);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case ZHI_XU_ZHI_YIN:
        action = newAction(ACTION_MAGIC_SKILL, ZHI_XU_ZHI_YIN);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case YING_LING_ZHAO_HUAN:
        respond = newRespond(YING_LING_ZHAO_HUAN);
        respond->add_args(1);
        if(selectedCards.size()>0)
        {
            respond->add_args(1);
			respond->add_card_ids(selectedCards[0]->getID());
        }
        else {
            respond->add_args(0);
        }
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
}

void NvWuShen::onCancelClicked()
{
    Role::onCancelClicked();

    static network::Respond* respond;

    switch(state)
    {
    case JUN_SHEN_WEI_GUANG_2:
        JunShenWeiGuang();
        break;
    case YING_LING_ZHAO_HUAN:
        respond = newRespond(YING_LING_ZHAO_HUAN);
        respond->add_args(0);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case ZHI_XU_ZHI_YIN:
        normal();
        break;
    }
}

void NvWuShen::askForSkill(network::Command* cmd)
{
    gui->alert();
    switch (cmd->respond_id())
    {
    case JUN_SHEN_WEI_GUANG:
        JunShenWeiGuang();
        break;
    case YING_LING_ZHAO_HUAN:
        YingLingZhaoHuan();
        break;
    default:
        Role::askForSkill(cmd);
    }
}


