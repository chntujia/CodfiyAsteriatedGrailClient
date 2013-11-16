#include "YuanSu.h"

enum CAUSE {
    FENG_REN = 1101,
    BING_DONG = 1102,
    HUO_QIU = 1103,
    YUN_SHI = 1104,
    LEI_JI = 1105,
    YUAN_SU_DIAN_RAN = 1106,
    YUE_GUANG = 1107,
    YUAN_SU_XI_SHOU = 1108,
    STATE_YUAN_MO_FA = 1151,
    STATE_YUAN_MO_FA_2 = 1152,
};

YuanSu::YuanSu()
{
    makeConnection();
setMyRole(this);

    Button *yuanSuFaShu,*yuanSuDianRan,*yueGuang;
    yuanSuFaShu=new Button(3,QStringLiteral("元素法术"));
    buttonArea->addButton(yuanSuFaShu);
    connect(yuanSuFaShu,SIGNAL(buttonSelected(int)),this,SLOT(YuanSuFaShu1()));

    yuanSuDianRan=new Button(4,QStringLiteral("元素点燃"));
    buttonArea->addButton(yuanSuDianRan);
    connect(yuanSuDianRan,SIGNAL(buttonSelected(int)),this,SLOT(YuanSuDianRan()));

    yueGuang=new Button(5,QStringLiteral("月光"));
    buttonArea->addButton(yueGuang);
    connect(yueGuang,SIGNAL(buttonSelected(int)),this,SLOT(YueGuang()));
}

void YuanSu::YuanSuFaShu1()
{
    state=STATE_YUAN_MO_FA;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    tipArea->setMsg(QStringLiteral("请先选择咏系牌和法伤目标"));
    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableProperty(QStringLiteral("咏"));
    handArea->disableElement("darkness");
    handArea->disableMagic();
}

void YuanSu::YuanSuFaShu2()
{
    state=STATE_YUAN_MO_FA_2;
    magicCard=handArea->getSelectedCards().takeFirst();
    magicTargetID=playerArea->getSelectedPlayers().takeFirst()->getID();

    playerArea->reset();
    handArea->reset();
    handArea->setQuota(0,1);
    handArea->enableElement(magicCard->getElement());
    foreach(CardItem*ptr, handArea->getHandCardItems())
        if(ptr->getCard()->getID()==magicCard->getID())
        {
            ptr->setEnabled(0);
            ptr->setY(-20);
        }

    if(magicCard->getElement()=="water")
    {
        playerArea->enableAll();        
        decisionArea->disable(0);
        tipArea->setMsg(QStringLiteral("请选择治疗目标；若多弃1张同系牌，本次伤害+1"));
    }
    else
        tipArea->setMsg(QStringLiteral("若多弃1张同系牌，本次伤害+1"));
}

void YuanSu::YuanSuDianRan()
{
    state=YUAN_SU_DIAN_RAN;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    playerArea->enableAll();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void YuanSu::YueGuang()
{
    state=YUE_GUANG;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    playerArea->enableAll();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void YuanSu::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
//元素法术
    foreach(Card*ptr, dataInterface->getHandCards())
        if (ptr->getType()=="attack"&&ptr->getProperty()==QStringLiteral("咏")&&ptr->getElement()!="darkness")
            buttonArea->enable(3);
//元素点燃
    if(myself->getToken(0)==3)
        buttonArea->enable(4);
//月光
    if(myself->getGem()>0)
        buttonArea->enable(5);
    unactionalCheck();
}

void YuanSu::magicAction()
{
    Role::magicAction();
    Player* myself=dataInterface->getMyself();
//元素法术
    foreach(Card*ptr, dataInterface->getHandCards())
        if (ptr->getType()=="attack"&&ptr->getProperty()==QStringLiteral("咏")&&ptr->getElement()!="darkness")
            buttonArea->enable(3);
//元素点燃
    if(myself->getToken(0)==3)
        buttonArea->enable(4);
//月光
    if(myself->getGem()>0)
        buttonArea->enable(5);
}

void YuanSu::cardAnalyse()
{
    Role::cardAnalyse();

    switch (state)
    {
//元素法术1
    case STATE_YUAN_MO_FA:
        playerArea->enableAll();
        break;
    }
}

void YuanSu::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    QString command;
    QString cardID;
    QString sourceID;
    QString targetID;
    QString text;    
    bool water=false;
    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;
    int magic;

    switch(state)
    {
//额外行动询问
    case 42:
        text=tipArea->getBoxCurrentText();
        switch(text[0].digitValue())
        {
        case 1:
            earth=false;
            respond = newRespond(YUN_SHI);
            respond->add_args(1);
            emit sendCommand(network::MSG_RESPOND, respond);
            magicAction();
            break;
        case 2:
            ignite=false;
            respond = newRespond(YUAN_SU_DIAN_RAN);
            respond->add_args(1);
            emit sendCommand(network::MSG_RESPOND, respond);
            magicAction();
            break;
        case 3:
            wind=false;
            respond = newRespond(FENG_REN);
            respond->add_args(1);
            emit sendCommand(network::MSG_RESPOND, respond);
            attackAction();
            break;
        }
        break;
//元素法术1
    case STATE_YUAN_MO_FA:
        YuanSuFaShu2();
        break;
//元素法术2
    case STATE_YUAN_MO_FA_2:
        if(magicCard->getElement()=="wind"){
            magic =FENG_REN;
            wind=true;
        }
        else if(magicCard->getElement()=="water"){
            magic = BING_DONG;
            water=true;
        }
        else if(magicCard->getElement()=="fire") {
            magic = HUO_QIU;
        }
        else if(magicCard->getElement()=="earth"){
            magic = YUN_SHI;
            earth=true;
        }
        else
            magic = LEI_JI;
        action = newAction(ACTION_MAGIC_SKILL, magic);
        action->add_card_ids(magicCard->getID());
        action->add_dst_ids(magicTargetID);
        if(handArea->getSelectedCards().size()>0){
            action->add_card_ids(selectedCards[0]->getID());
        }
        if(water)
            action->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//元素点燃
    case YUAN_SU_DIAN_RAN:
        action = newAction(ACTION_MAGIC_SKILL, YUAN_SU_DIAN_RAN);
        action->add_dst_ids(selectedPlayers[0]->getID());
        ignite=true;
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//月光
    case YUE_GUANG:
        action = newAction(ACTION_MAGIC_SKILL, YUE_GUANG);
        action->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    }
}

void YuanSu::onCancelClicked()
{
    Role::onCancelClicked();
    switch(state)
    {
    case STATE_YUAN_MO_FA:
    case YUAN_SU_DIAN_RAN:
    case YUE_GUANG:
        if(actionFlag==0)
            normal();
        else if(actionFlag==2)
            magicAction();
        break;
    case STATE_YUAN_MO_FA_2:
        YuanSuFaShu1();
        break;
    }
}
void YuanSu::additionalAction()
{
    //Role::additionalAction();
    if(earth)
        tipArea->addBoxItem(QStringLiteral("1.法术行动（陨石）"));
    if(ignite)
        tipArea->addBoxItem(QStringLiteral("2.法术行动（元素点燃）"));
    if(wind)
        tipArea->addBoxItem(QStringLiteral("3.攻击行动（风刃）"));
}

void YuanSu::turnBegin()
{
    Role::turnBegin();
    wind=false;
    earth=false;
    ignite=false;
}
