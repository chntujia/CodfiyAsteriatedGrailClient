#include "MoQiang.h"

enum CAUSE{
    AN_ZHI_JIE_FANG=2901,
    HUAN_YING_XING_CHEN=2902,
    HEI_AN_SHU_FU=2903,
    AN_ZHI_ZHANG_BI=2904,
    CHONG_YING=2905,
    QI_HEI_ZHI_QIANG=2906

};

MoQiang::MoQiang()
{
    makeConnection();
    setMyRole(this);

    Button *chongying;
    chongying=new Button(3,QStringLiteral("充盈"));
    buttonArea->addButton(chongying);
    connect(chongying,SIGNAL(buttonSelected(int)),this,SLOT(ChongYing()));
}

void MoQiang::normal()
{
    Role::normal();
    handArea->disableMagic();
    if(!jieFangFirst)
    {
        if(handArea->checkElement("thunder") || handArea->checkType("magic"))
            buttonArea->enable(3);
    }
    unactionalCheck();
}

void MoQiang::AnZhiJieFang()
{
    state=AN_ZHI_JIE_FANG;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动暗之解放？"));
    decisionArea->enable(1);
    decisionArea->enable(0);

}


void MoQiang::HuanYingXingChen()
{
    state=HUAN_YING_XING_CHEN;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动幻影星辰？"));
    playerArea->enableAll();
    playerArea->setQuota(1);
    decisionArea->enable(1);

}

void MoQiang::AnZhiBiZhang()
{
    state=AN_ZHI_ZHANG_BI;
    tipArea->setMsg(QStringLiteral("是否发动暗之壁障？"));
    handArea->setQuota(1,7);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("thunder");
    handArea->enableMagic();
}

void MoQiang::QiHeiZhiQiang()
{
    state= QI_HEI_ZHI_QIANG;
    tipArea->reset();
    tipArea->setMsg(QStringLiteral("是否发动漆黑之枪？如是请选择发动能量数："));
    decisionArea->enable(0);
    decisionArea->enable(1);

    Player* myself=dataInterface->getMyself();
    int min=myself->getEnergy();
    for(;min>0;min--)
        tipArea->addBoxItem(QString::number(min));
    tipArea->showBox();
}

void MoQiang::ChongYing()
{
    state=CHONG_YING;
    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    handArea->enableElement("thunder");
    handArea->enableMagic();
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void MoQiang::cardAnalyse()
{
    QList<Card*>selectedCards;
    Role::cardAnalyse();
    selectedCards=handArea->getSelectedCards();
    switch(state)
    {
    case AN_ZHI_ZHANG_BI:
      cardReady=false;
      if(selectedCards.size()==0)
          decisionArea->disable(0);
      else
      {
          foreach(Card*ptr,selectedCards)
              for(int i=0;i<selectedCards.size();i++)
              {
                  if(ptr->getElement()!=selectedCards[i]->getElement())
                  {
                      if(ptr->getType()!="magic")
                      {
                          playerArea->reset();
                          decisionArea->disable(0);
                          cardReady=true;
                          break;
                      }
                  }
               }
          if(!cardReady)
              decisionArea->enable(0);
        }


      break;

   case CHONG_YING:
        decisionArea->enable(0);
        break;
    }
}

void MoQiang::playerAnalyse()
{
    Role::playerAnalyse();
    switch(state)
    {
    case HUAN_YING_XING_CHEN:
        decisionArea->enable(0);
        break;
    }
}


void MoQiang::turnBegin()
{
    Role::turnBegin();
    jieFangFirst=false;
}

void MoQiang::askForSkill(Command* cmd)
{
    switch(cmd->respond_id())
    {
    case  AN_ZHI_JIE_FANG:
        AnZhiJieFang();
        break;
    case  HUAN_YING_XING_CHEN:
        HuanYingXingChen();
        break;
    case  QI_HEI_ZHI_QIANG:
        QiHeiZhiQiang();
        break;
    case  AN_ZHI_ZHANG_BI:
        AnZhiBiZhang();
        break;
    default:
        Role::askForSkill(cmd);
    }
}


void MoQiang::onOkClicked()
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
      case AN_ZHI_JIE_FANG:
        respond = new Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(AN_ZHI_JIE_FANG);
        respond->add_args(1);
        jieFangFirst=true;
        gui->reset();
        emit sendCommand(network::MSG_RESPOND, respond);
        break;

      case HUAN_YING_XING_CHEN:
        respond = newRespond(HUAN_YING_XING_CHEN);
        respond->add_args(2);
        respond->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
      case AN_ZHI_ZHANG_BI:
        respond = newRespond(AN_ZHI_ZHANG_BI);
        respond->add_args(1);
        for(int i=0;i<selectedCards.size();i++)
        {
            respond->add_card_ids(selectedCards[i]->getID());

        }
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

    case QI_HEI_ZHI_QIANG:
        respond = newRespond(QI_HEI_ZHI_QIANG);
        respond->add_args(1);
        respond->add_args(tipArea->getBoxCurrentText().toInt());
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

      case CHONG_YING:
        action = newAction(ACTION_MAGIC_SKILL,CHONG_YING);
        action->add_card_ids(selectedCards[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    }
}

void MoQiang::onCancelClicked()
{
    Role::onCancelClicked();

    network::Respond* respond;

    switch(state)
    {

    case AN_ZHI_JIE_FANG:
        respond = new Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(AN_ZHI_JIE_FANG);
        respond->add_args(0);
        gui->reset();
        emit sendCommand(network::MSG_RESPOND, respond);

        break;

    case HUAN_YING_XING_CHEN:
        respond = newRespond(HUAN_YING_XING_CHEN);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

        case AN_ZHI_ZHANG_BI:
        respond = newRespond(AN_ZHI_ZHANG_BI);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

      case QI_HEI_ZHI_QIANG:
        respond = newRespond(QI_HEI_ZHI_QIANG);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

      case CHONG_YING:
        normal();
        break;
    }
}

