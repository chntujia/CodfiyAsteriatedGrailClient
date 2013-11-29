#include "XianZhe.h"

enum {
    ZHI_HUI_FA_DIAN = 1701,
    MO_DAO_FA_DIAN = 1702,
    SHENG_JIE_FA_DIAN = 1703,
    FA_SHU_FAN_TAN = 1704,
};

XianZhe::XianZhe()
{
    makeConnection();
    setMyRole(this);

    Button *moDaoFaDian;
    moDaoFaDian=new Button(3, QStringLiteral("魔道法典"));
    buttonArea->addButton(moDaoFaDian);
    connect(moDaoFaDian,SIGNAL(buttonSelected(int)),this,SLOT(MoDaoFaDian()));

    Button *shengJieFaDian;
    shengJieFaDian=new Button(4, QStringLiteral("圣洁法典"));
    buttonArea->addButton(shengJieFaDian);
    connect(shengJieFaDian,SIGNAL(buttonSelected(int)),this,SLOT(ShengJieFaDian()));
}

void XianZhe::normal()
{
    Role::normal();
    Player *myself=dataInterface->getMyself();

    int elements = handArea->getElementCount();
    // 有能量              && 至少有2系牌
    if(myself->getGem()>0 && elements>1)
    {
        buttonArea->enable(3);
        // 至少有3系牌
        if(elements>2)
            buttonArea->enable(4);
    }
    unactionalCheck();
}

void XianZhe::FaShuFanTan()
{
    state=FA_SHU_FAN_TAN;
    tipArea->setMsg(QStringLiteral("是否发动法术反弹？"));
    decisionArea->enable(0);
    decisionArea->enable(1);

    handArea->enableAll();
    handArea->setQuota(2,7);
    decisionArea->enable(1);
    decisionArea->disable(0);
}

void XianZhe::MoDaoFaDian()
{
    state=MO_DAO_FA_DIAN;

    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    handArea->enableAll();
    handArea->setQuota(2,7);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void XianZhe::ShengJieFaDian()
{
    state=SHENG_JIE_FA_DIAN;

    playerArea->reset();
    handArea->reset();
    tipArea->reset();
    tipArea->setMsg(QStringLiteral("只仔细确认选取的目标！！！"));

    handArea->enableAll();
    handArea->setQuota(3,7);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void XianZhe::cardAnalyse()
{
    Role::cardAnalyse();
    QList<Card*> selectedCards=handArea->getSelectedCards();

    switch(state)
    {
    case FA_SHU_FAN_TAN:
        cardReady=false;
        if(selectedCards.size()<2)
            decisionArea->disable(0);
        else
        {
            foreach(Card*ptr,selectedCards)
                for(int i=0;i<selectedCards.size();i++){
                    if(ptr->getElement()!=selectedCards[i]->getElement() && ptr->getID()!=selectedCards[i]->getID()){
                        playerArea->reset();
                        decisionArea->disable(0);
                        cardReady=true;
                        break;
                    }
                }
            if(!cardReady)
                playerArea->enableAll();
        }
        break;
    case MO_DAO_FA_DIAN:
        cardReady=false;
        if(selectedCards.size()<2)
            decisionArea->disable(0);
        else
        {
            foreach(Card*ptr,selectedCards)
                for(int i=0;i<selectedCards.size();i++){
                    if(ptr->getElement()==selectedCards[i]->getElement() && ptr->getID()!=selectedCards[i]->getID()){
                        playerArea->reset();
                        decisionArea->disable(0);
                        cardReady=true;
                        break;
                    }
                }
            if(!cardReady)
                playerArea->enableAll();
        }
        break;
    case SHENG_JIE_FA_DIAN:
        cardReady=false;
        if(selectedCards.size()<3)
            decisionArea->disable(0);
        else
        {
            foreach(Card*ptr,selectedCards)
                for(int i=0;i<selectedCards.size();i++){
                    if(ptr->getElement()==selectedCards[i]->getElement() && ptr->getID()!=selectedCards[i]->getID()){
                        playerArea->reset();
                        decisionArea->disable(0);
                        cardReady=true;
                        break;
                    }
                }
            if(!cardReady){
                playerArea->enableAll();
                decisionArea->enable(0);
            }
        }
        break;
    }
}

void XianZhe::playerAnalyse()
{
    Role::playerAnalyse();
    QList<Card*> selectedCards=handArea->getSelectedCards();
    switch(state)
    {
    case FA_SHU_FAN_TAN:
    case MO_DAO_FA_DIAN:
        playerArea->setQuota(1);
        cardReady=false;
        break;
    case SHENG_JIE_FA_DIAN:
        playerArea->setQuota(0,selectedCards.size()-2);
        cardReady=false;
        break;
    }
}

void XianZhe::onOkClicked()
{
    Role::onOkClicked();
    QList<Player*>selectedPlayers;
    QList<Card*>selectedCards;

    QString targetID;
    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;

    switch(state)
    {
    case FA_SHU_FAN_TAN:
        respond = newRespond(FA_SHU_FAN_TAN);
        respond->add_dst_ids(selectedPlayers[0]->getID());
        foreach(Card*ptr,selectedCards){
            respond->add_card_ids(ptr->getID());
        }
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case MO_DAO_FA_DIAN:
        action = newAction(ACTION_MAGIC_SKILL, MO_DAO_FA_DIAN);
        action->add_dst_ids(selectedPlayers[0]->getID());

        foreach(Card*ptr,selectedCards){
            action->add_card_ids(ptr->getID());
        }
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case SHENG_JIE_FA_DIAN:
        action = newAction(ACTION_MAGIC_SKILL, SHENG_JIE_FA_DIAN);
        foreach(Card*ptr,selectedCards){
            action->add_card_ids(ptr->getID());
        }
        foreach(Player*ptr,selectedPlayers)
            action->add_dst_ids(ptr->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    }
}

void XianZhe::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;

    network::Respond* respond;
    switch(state)
    {
    case FA_SHU_FAN_TAN:
        respond = newRespond(FA_SHU_FAN_TAN);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case MO_DAO_FA_DIAN:
    case SHENG_JIE_FA_DIAN:
        normal();
        break;
    }
}

void XianZhe::askForSkill(network::Command* cmd)
{
    if(cmd->respond_id() == FA_SHU_FAN_TAN)
        FaShuFanTan();
    else
        Role::askForSkill(cmd);
}


