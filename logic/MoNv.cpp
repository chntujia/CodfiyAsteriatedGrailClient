

#include "MoNv.h"
enum CAUSE{
    CANG_YAN_FA_DIAN=3001,
    TIAN_HUO_DUAN_KONG=3002,
    MO_NV_ZHI_NU=3003,
    MO_NV_ZHI_NU_ATTACK=30031,
    TI_SHEN_WAN_OU=3004,
    YONG_SHENG_YIN_SHI_JI=3005,
    TONG_KU_LIAN_JIE=3006,
    TONG_KU_LIAN_JIE_CARD=30061,
    MO_NENG_FAN_ZHUAN=3007
};

MoNv::MoNv()
{
    makeConnection();
    setMyRole(this);
    Button *cangYanFaDian, *tianHuoDuanKong, *tongKuLianJie;
    cangYanFaDian = new Button(3,QStringLiteral("苍炎法典"));
    buttonArea->addButton(cangYanFaDian);
    connect(cangYanFaDian,SIGNAL(buttonSelected(int)),this,SLOT(CangYanFaDian()));

    tianHuoDuanKong = new Button(4,QStringLiteral("天火断空"));
    buttonArea->addButton(tianHuoDuanKong);
    connect(tianHuoDuanKong,SIGNAL(buttonSelected(int)),this,SLOT(TianHuoDuanKong()));

    tongKuLianJie = new Button(5,QStringLiteral("痛苦链接"));
    buttonArea->addButton(tongKuLianJie);
    connect(tongKuLianJie,SIGNAL(buttonSelected(int)),this,SLOT(TongKuLianJie()));
}

int MoNv::checkFire()
{
    Player* myself=dataInterface->getMyself();
    SafeList<Card*> handcards = dataInterface->getHandCards();
    int fire = 0;
    for(int i=0; i<handcards.size();i++)
    {
        if(handcards[i]->getElement() == QStringLiteral("fire"))
            fire ++;
        if(myself->getTap() && handcards[i]->getType() == "attack" &&
                (handcards[i]->getElement() == "thunder" || handcards[i]->getElement() == "earth" || handcards[i]->getElement() == "wind"))
            fire ++;
    }
    return fire;
}

void MoNv::enbleFire()
{
    Player* myself=dataInterface->getMyself();
    if(myself->getTap())
    {
        handArea->enableTypeAndElement(QStringLiteral("attack"), QStringLiteral("thunder"));
        handArea->enableTypeAndElement(QStringLiteral("attack"), QStringLiteral("wind"));
        handArea->enableTypeAndElement(QStringLiteral("attack"), QStringLiteral("earth"));
    }
    handArea->enableElement(QStringLiteral("fire"));
}

void MoNv::enbleFireAttack(QString element)
{
    Player* myself=dataInterface->getMyself();
    if(myself->getTap())
    {
        if(element == "water")
            handArea->enableElement(element);
        if(element == "fire")
        {
            handArea->enableElement("fire");
            handArea->enableElement("thunder");
            handArea->enableElement("earth");
            handArea->enableElement("wind");
        }
    }
    else
    {
        handArea->enableElement(element);
    }
}

void MoNv::attacked(QString element,int hitRate)
{
    Role::attacked(element, hitRate);

    handArea->disableAttack();
    if(hitRate==0)
    {
        this->enbleFireAttack(element);
        handArea->enableElement("darkness");
    }
}

void MoNv::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    int fire = checkFire();
    if(fire > 0)
        buttonArea->enable(3);
    if(fire > 1 && (myself->getToken(0)>0 || myself->getTap()))
        buttonArea->enable(4);
    if(myself->getEnergy()>0)
        buttonArea->enable(5);
    unactionalCheck();
}


void MoNv::CangYanFaDian()
{
    state = CANG_YAN_FA_DIAN;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    this->enbleFire();

    handArea->setQuota(1);
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void MoNv::TianHuoDuanKong()
{
    state = TIAN_HUO_DUAN_KONG;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    this->enbleFire();

    handArea->setQuota(2);
    playerArea->setQuota(1);

    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void MoNv::TongKuLianJie()
{
    state = TONG_KU_LIAN_JIE;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->enableEnemy();

    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void MoNv::cardAnalyse()
{
    Role::cardAnalyse();

    switch (state)
    {
    case CANG_YAN_FA_DIAN:
    case TIAN_HUO_DUAN_KONG:
        playerArea->enableAll();
        break;
    case TI_SHEN_WAN_OU:
        playerArea->enableMate();
        break;
    case MO_NENG_FAN_ZHUAN:
        playerArea->enableEnemy();
        break;
    }
}

void MoNv::MoNvZhiNu()
{
    state = MO_NV_ZHI_NU;
    gui->reset();
    tipArea->setMsg(QStringLiteral("发动【魔女之怒】，并选择要摸取的手牌数量"));
    for(int i =0; i < 3 ; i ++)
        tipArea->addBoxItem(QString::number(i));
    tipArea->showBox();
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void MoNv::TiShenWanOu()
{
    state=TI_SHEN_WAN_OU;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动替身玩偶"));
    handArea->setQuota(1);
    handArea->enableMagic();
    decisionArea->enable(1);
}

void MoNv::MoNengFanZhuan()
{
    state=MO_NENG_FAN_ZHUAN;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动魔能反转"));
    handArea->setQuota(2,9);
    handArea->enableMagic();
    decisionArea->enable(1);
}

void MoNv::askForSkill(network::Command* cmd)
{
    switch (cmd->respond_id()) {
    case MO_NV_ZHI_NU:
        MoNvZhiNu();
        break;
    case TI_SHEN_WAN_OU:
        TiShenWanOu();
        break;
    case MO_NENG_FAN_ZHUAN:
        MoNengFanZhuan();
        break;
    default:
        Role::askForSkill(cmd);
        break;
    }
}

void MoNv::onOkClicked()
{
    Player* myself=dataInterface->getMyself();
    SafeList<Card*> selectedCards;
    SafeList<Player*>selectedPlayers;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;

    try{
    switch(state)
    {
    case 1:
        if(selectedCards[0]->getType()=="attack" && myself->getTap() &&
                (selectedCards[0]->getElement() == "thunder" || selectedCards[0]->getElement() == "earth" || selectedCards[0]->getElement() == "wind")){
            state = MO_NV_ZHI_NU_ATTACK;
            action = newAction(ACTION_ATTACK_SKILL, MO_NV_ZHI_NU_ATTACK);
            action->add_dst_ids(selectedPlayers[0]->getID());
            action->add_card_ids(selectedCards[0]->getID());
            usedAttack=true;
            usedMagic=usedSpecial=false;
            gui->reset();
            emit sendCommand(network::MSG_ACTION, action);
        }
        break;
    case TI_SHEN_WAN_OU:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(TI_SHEN_WAN_OU);
        respond->add_card_ids(selectedCards[0]->getID());
        respond->add_dst_ids(selectedPlayers[0]->getID());
        respond->add_args(1);// 1表示选择发动
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case MO_NENG_FAN_ZHUAN:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(MO_NENG_FAN_ZHUAN);
        respond->add_dst_ids(selectedPlayers[0]->getID());
        for(int i =0; i < selectedCards.size();i++)
            respond->add_card_ids(selectedCards[i]->getID());
        respond->add_args(1);// 1表示选择发动
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case MO_NV_ZHI_NU:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(MO_NV_ZHI_NU);
        respond->add_args(1);// 1表示选择发动
        respond->add_args(tipArea->getBoxCurrentText().toInt());
        emit sendCommand(network::MSG_RESPOND, respond);
        start =true;
		gui->reset();
        break;
    case CANG_YAN_FA_DIAN:
        action = newAction(ACTION_MAGIC_SKILL, CANG_YAN_FA_DIAN);
        action->set_src_id(myID);

        action->add_card_ids(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());

        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case TIAN_HUO_DUAN_KONG:
        action = newAction(ACTION_MAGIC_SKILL, TIAN_HUO_DUAN_KONG);
        action->set_src_id(myID);

        action->add_card_ids(selectedCards[0]->getID());
        action->add_card_ids(selectedCards[1]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());

        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case TONG_KU_LIAN_JIE:
        action = newAction(ACTION_MAGIC_SKILL, TONG_KU_LIAN_JIE);
        action->set_src_id(myID);
        action->add_dst_ids(selectedPlayers[0]->getID());

        action->add_args(1);

        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    }
    }catch(int error){
        logic->onError(error);
    }
    Role::onOkClicked();
}

void MoNv::onCancelClicked()
{
    Role::onCancelClicked();

    network::Respond* respond;

    switch(state)
    {
    case MO_NV_ZHI_NU:
    case TI_SHEN_WAN_OU:
    case MO_NENG_FAN_ZHUAN:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(state);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case CANG_YAN_FA_DIAN:
    case TIAN_HUO_DUAN_KONG:
    case TONG_KU_LIAN_JIE:
        normal();
        break;
    }
}
