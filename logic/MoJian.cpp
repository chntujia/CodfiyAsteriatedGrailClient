#include "MoJian.h"

enum CAUSE{
    XIU_LUO_LIAN_ZHAN=901,
    AN_YING_NING_JU=902,
    AN_YING_ZHI_LI=903,
    AN_YING_KANG_JU=904,
    AN_YING_LIU_XING=905,
    HEI_AN_ZHEN_CHAN=906
};


MoJian::MoJian()
{
    makeConnection();
    setMyRole(this);
    Button *anYingLiuXing;
    anYingLiuXing=new Button(3,QStringLiteral("暗影流星"));
    buttonArea->addButton(anYingLiuXing);
    connect(anYingLiuXing,SIGNAL(buttonSelected(int)),this,SLOT(AnYingLiuXing()));
}

void MoJian::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    int count=0;
    handArea->disableMagic();
    if(myself->getTap())
        foreach(Card*ptr,dataInterface->getHandCards())
        {
            if(ptr->getType()=="magic")
                count++;
            if(count>=2){
                buttonArea->enable(3);
                break;
            }
        }
    unactionalCheck();
}

void MoJian::XiuLuoLianZhan()
{

    state=XIU_LUO_LIAN_ZHAN;
    handArea->setQuota(1);
    handArea->enableElement("fire");
    handArea->disableMagic();
    playerArea->enableEnemy(); //added
    playerArea->setQuota(1);
    decisionArea->enable(3);
}

void MoJian::AnYingNingJu()
{
    state = AN_YING_NING_JU;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动暗影凝聚？"));

    decisionArea->enable(0);
    decisionArea->enable(1);
}

void MoJian::AnYingLiuXing()
{
    state=AN_YING_LIU_XING;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    handArea->setQuota(2);
    handArea->enableMagic();
    playerArea->setQuota(1);
    decisionArea->enable(1);
}

void MoJian::cardAnalyse()
{
    Role::cardAnalyse();

    switch (state)
    {
      case AN_YING_LIU_XING:
        playerArea->enableAll();
        break;
      case XIU_LUO_LIAN_ZHAN:
         playerArea->enableEnemy();
         decisionArea->enable(0);
         break;
    }
}

void MoJian::onOkClicked()
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
  case  XIU_LUO_LIAN_ZHAN:
        action = newAction(network::ACTION_ATTACK);
        action->add_card_ids(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());
        usedAttack=true;
        usedMagic=usedSpecial=false;
        gui->reset();
        emit sendCommand(network::MSG_ACTION, action);
        break;

//暗影流星
    case AN_YING_LIU_XING:
        action = newAction(ACTION_MAGIC_SKILL,AN_YING_LIU_XING);
        action->add_card_ids(selectedCards[0]->getID());
        action->add_card_ids(selectedCards[1]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;

 //【暗影凝聚
      case AN_YING_NING_JU:
        start = true;
        respond = new Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(skillCmd.respond_id());
        respond->add_args(1);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

    }
}

void MoJian::onCancelClicked()
{
    Role::onCancelClicked();
    QList<Player*>selectedPlayers;
    selectedPlayers=playerArea->getSelectedPlayers();  //added

    network::Respond* respond;
    switch(state)
    {
 //【修罗连斩】
   case XIU_LUO_LIAN_ZHAN:

//暗影流星
    case AN_YING_LIU_XING:
        normal();
        break;
//【暗影凝聚】
      case AN_YING_NING_JU:
        respond = new Respond();
        respond->set_src_id(myID);
        respond ->set_respond_id(AN_YING_NING_JU);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
}

void MoJian::attackAction()
{
    //若是连续技的额外行动，则只能用火系
    if(XIU_LUO_LIAN_ZHAN == chosenAction){
       XiuLuoLianZhan();
    }
    else{
        Role::attackAction();
    }
}

void MoJian::askForSkill(network::Command* cmd)
{
    if(cmd->respond_id() == AN_YING_NING_JU)
        AnYingNingJu();
    else
        Role::askForSkill(cmd);
}

void MoJian::attacked(QString element, int hitRate)
{
    Role::attacked(element,hitRate);
    if(isMyTurn)
        handArea->disableMagic();
}


