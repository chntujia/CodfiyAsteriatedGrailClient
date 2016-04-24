#include "JianDi.h"

enum CAUSE{
    JIAN_HUN_SHOU_HU=1901,
    YANG_GON=1902,
    JIAN_QI_ZHAN=1903,
    TIAN_SHI_YU_E_MO=1904,
    TIAN_SHI_ZHI_HUN=1905,
    E_MO_ZHI_HUN=1906,
    BU_QU_YI_ZHI=1907
};


JianDi::JianDi()
{
    makeConnection();
    setMyRole(this);

    Button *checkCover;
    checkCover = new Button(10,QStringLiteral("查看剑魂"));
    buttonArea->addOutsideTurnButton(checkCover);

    checkCover->setVisible(true);
    checkCover->setEnabled(true);
    connect(checkCover,SIGNAL(buttonSelected(int)),gui,SLOT(showCoverArea()));
    connect(checkCover,SIGNAL(buttonUnselected(int)),gui,SLOT(closeCoverArea()));
}

void  JianDi::BuQiYiZhi()
{
    state=BU_QU_YI_ZHI;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动不屈意志？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}


void JianDi::JianHunShouHu()
{
    state=JIAN_HUN_SHOU_HU;
    tipArea->setMsg(QStringLiteral("是否发动剑魂守护？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
    handArea->disableAll();
}

void JianDi::JianQiZhan(int targetID)
{
    state=JIAN_QI_ZHAN;
    Player* myself=dataInterface->getMyself();
    jianqiID=targetID;      //去除变量jianqiID
    tipArea->setMsg(QStringLiteral("请选择要移除的剑气数："));
    int min=myself->getToken(0)<3?myself->getToken(0):3;
    for(;min>0;min--)
        tipArea->addBoxItem(QString::number(min));
    tipArea->showBox();
    decisionArea->enable(1);

   playerArea->enableAll();
   playerArea->disablePlayerItem(targetID);
   playerArea->setQuota(1);
    decisionArea->disable(0);
    handArea->disableAll();
}

void JianDi::TianShiZhiHun()
{
    state=TIAN_SHI_ZHI_HUN;
    tipArea->setMsg(QStringLiteral("是否发动天使之魂？如发动请选择剑魂："));
    decisionArea->enable(1);
    decisionArea->disable(0);
    gui->showCoverArea(true);
    HandArea *coverArea = gui->getCoverArea();
    coverArea->reset();
    coverArea->enableAll();
    coverArea->setQuota(1);
}

void JianDi::EMoZhiHun()
{
    state=E_MO_ZHI_HUN;
    tipArea->setMsg(QStringLiteral("是否发动恶魔之魂？如发动请选择剑魂："));
    decisionArea->enable(1);
    decisionArea->disable(0);
    gui->showCoverArea(true);
    HandArea *coverArea = gui->getCoverArea();
    coverArea->reset();
    coverArea->enableAll();
    coverArea->setQuota(1);
}

void JianDi::askForSkill(network::Command* cmd)
{

    if(cmd->respond_id()==JIAN_HUN_SHOU_HU)
         JianHunShouHu();
    else if(cmd->respond_id()==BU_QU_YI_ZHI)
         BuQiYiZhi();
    else if(cmd->respond_id()==JIAN_QI_ZHAN)
         JianQiZhan(cmd->args(0));
    else if(cmd->respond_id()==TIAN_SHI_ZHI_HUN)
         TianShiZhiHun();
    else if(cmd->respond_id()==E_MO_ZHI_HUN)
         EMoZhiHun();
    else
         Role::askForSkill(cmd);
}




void JianDi::coverCardAnalyse()
{
    switch(state)
    {
  case  TIAN_SHI_ZHI_HUN:
  case  E_MO_ZHI_HUN:
        decisionArea->enable(0);
        break;
    }
}


void JianDi::onOkClicked()
{
    Role::onOkClicked();

    SafeList<Card*> selectedCoverCards = coverArea->getSelectedCards();
    SafeList<Player*> selectedPlayers = playerArea->getSelectedPlayers();

    network::Respond* respond;
    try{
    switch(state)
    {
      case JIAN_HUN_SHOU_HU:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(JIAN_HUN_SHOU_HU);
        respond->add_args(1);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

      case BU_QU_YI_ZHI:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(BU_QU_YI_ZHI);
        respond->add_args(1);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

        case JIAN_QI_ZHAN:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(JIAN_QI_ZHAN);
        respond->add_dst_ids(selectedPlayers[0]->getID());
        respond->add_args(1);
        jianqiPoint=tipArea->getBoxCurrentText();
        respond->add_args(jianqiPoint.toInt());
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

      case TIAN_SHI_ZHI_HUN:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(TIAN_SHI_ZHI_HUN);
        respond->add_args(1);
        respond->add_card_ids(selectedCoverCards[0]->getID());
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

      case E_MO_ZHI_HUN:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(E_MO_ZHI_HUN);
        respond->add_args(1);
        respond->add_card_ids(selectedCoverCards[0]->getID());
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
    gui->showCoverArea(false);
    }catch(int error){
        logic->onError(error);
    }
}

void JianDi::onCancelClicked()
{
    Role::onCancelClicked();

    network::Respond* respond;

    switch(state)
    {
    case JIAN_HUN_SHOU_HU:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(JIAN_HUN_SHOU_HU);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

    case BU_QU_YI_ZHI:
      respond = new network::Respond();
      respond->set_src_id(myID);
      respond->set_respond_id(BU_QU_YI_ZHI);
      respond->add_args(0);
      emit sendCommand(network::MSG_RESPOND, respond);
      gui->reset();
      break;

      case JIAN_QI_ZHAN:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(JIAN_QI_ZHAN);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

      case TIAN_SHI_ZHI_HUN:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->add_args(0);
        respond->set_respond_id(TIAN_SHI_ZHI_HUN);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

       case E_MO_ZHI_HUN:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(E_MO_ZHI_HUN);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
    gui->showCoverArea(false);
}
