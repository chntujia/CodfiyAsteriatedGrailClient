#include "AnSha.h"

enum CAUSE{
    FAN_SHI = 501,
    SHUI_YING = 502,
    QIAN_XING = 503,
};

AnSha::AnSha()
{
    makeConnection();
setMyRole(this);
}

void AnSha::ShuiYing()
{
    state=SHUI_YING;
    tipArea->setMsg(QStringLiteral("是否发动水影？"));
    handArea->setQuota(1,7);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("water");
}

void AnSha::QianXing()
{
    state=QIAN_XING;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动潜行？"));
    SafeList<Card*> handcards=dataInterface->getHandCards();
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
    }
    if(flag)
        decisionArea->enable(0);
}

void AnSha::askForSkill(network::Command* cmd)
{
    if(cmd->respond_id() == SHUI_YING)
        ShuiYing();
    else if(cmd->respond_id() == QIAN_XING)
        QianXing();
    else
        Role::askForSkill(cmd);
}

void AnSha::cardAnalyse()
{
    Role::cardAnalyse();

    switch (state)
    {
    //水影询问
    case SHUI_YING:
        decisionArea->enable(0);
        break;
    }
}

void AnSha::onOkClicked()
{
    Role::onOkClicked();
    SafeList<Card*> selectedCards;
    int i;
    network::Respond* respond;

    selectedCards=handArea->getSelectedCards();
    try{
    switch(state)
    {
    //水影询问
    case SHUI_YING:
        respond = new Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(SHUI_YING);
        for(i=0;i<selectedCards.length();i++)
        {
            respond->add_card_ids(selectedCards[i]->getID());
        }
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    //潜行询问
    case QIAN_XING:
        respond = new Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(QIAN_XING);
        respond->add_args(1);
        start=true;
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
    }catch(int error){
        logic->onError(error);
    }
}

void AnSha::onCancelClicked()
{
    Role::onCancelClicked();
    network::Respond* respond;
    switch(state)
    {
    //水影询问
    case SHUI_YING:
        respond = new Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(SHUI_YING);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    //潜行询问
    case QIAN_XING:
        respond = new Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(QIAN_XING);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
}

