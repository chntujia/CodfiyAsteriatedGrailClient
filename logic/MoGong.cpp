#include "MoGong.h"

enum CAUSE{
    MO_GUAN_CHONG_JI=2601,
    MO_GUAN_CHONG_JI_HIT=26011,
    LEI_GUANG_SAN_SHE =2602,
    LEI_GUANG_SAN_SHE_EXTRA=26021,
    DUO_CHONG_SHE_JI=2603,
    CHONG_NENG=2604,
    CHONG_NENG_GAI_PAI=26041,
    MO_YAN=2605,
    MO_YAN_GAI_PAI=26051,
    CHONG_NENG_MO_YAN=2606
};

MoGong::MoGong()
{
    makeConnection();
    setMyRole(this);

    Button *leiGuangSanShe, *checkCover;

    leiGuangSanShe=new Button(3,QStringLiteral("雷光散射"));
    buttonArea->addButton(leiGuangSanShe);
    connect(leiGuangSanShe,SIGNAL(buttonSelected(int)),this,SLOT(LeiGuangSanShe()));

    checkCover = new Button(10,QStringLiteral("查看充能"));
    buttonArea->addOutsideTurnButton(checkCover);
    checkCover->setVisible(true);
    checkCover->setEnabled(true);
    connect(checkCover,SIGNAL(buttonSelected(int)),gui,SLOT(showCoverArea()));
    connect(checkCover,SIGNAL(buttonUnselected(int)),gui,SLOT(closeCoverArea()));
}

void MoGong::normal()
{
    Role::normal();
    bool hasThunder=false;
    foreach(Card*ptr,dataInterface->getCoverCards())
        if(ptr->getElement()=="thunder")
        {
            hasThunder=true;
            break;
        }
    if(hasThunder && !ChongNengUsed)
        buttonArea->enable(3);
    unactionalCheck();
}

void MoGong::MoGuanChongJi()
{
    state=MO_GUAN_CHONG_JI;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动魔贯冲击？"));
    gui->showCoverArea(true);
    coverArea->enableElement("fire");
    coverArea->setQuota(1);
    decisionArea->enable(1);
}

void MoGong::MoGuanChongJiHit()
{
    state=MO_GUAN_CHONG_JI_HIT;
    gui->reset();
    tipArea->setMsg(QStringLiteral("攻击命中，额外移除1火系充能伤害加1"));
    gui->showCoverArea(true);
    coverArea->enableElement("fire");
    coverArea->setQuota(1);
    buttonArea->enable(1);  //??
    decisionArea->enable(1);
}

//【雷光散射】
void MoGong::LeiGuangSanShe()
{
    state=LEI_GUANG_SAN_SHE;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    tipArea->setMsg(QStringLiteral("请选择雷系充能"));
    gui->showCoverArea(true);
    coverArea->reset();
    coverArea->enableElement("thunder");
    int n = 0;
    foreach(Card*ptr,dataInterface->getCoverCards())
        if(ptr->getElement()=="thunder")
            n++;
    coverArea->setQuota(1,n);
    decisionArea->enable(1);
}
//【多重射击】
void MoGong::DuoChongSheJi()
{
    state=DUO_CHONG_SHE_JI;
    DuoChongSheJiUsed=true;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    tipArea->setMsg(QStringLiteral("请选择风系充能"));
    gui->showCoverArea(true);
    coverArea->reset();
    coverArea->enableElement("wind");
    coverArea->setQuota(1);
    decisionArea->disable(0);
    decisionArea->enable(3);  //added by Tony
}

void MoGong::ChongNengMoYan()
{
    state=CHONG_NENG_MO_YAN;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动充能/魔眼？"));
    Player* myself = dataInterface->getMyself();
        if(myself->getEnergy()>0)
           tipArea->addBoxItem(QStringLiteral("1.充能"));   //水晶都能用宝石替代
        if(myself->getGem()>0)
            tipArea->addBoxItem(QStringLiteral("2.魔眼"));
    tipArea->showBox();
    decisionArea->enable(1);
    decisionArea->enable(0);
}

//------弃牌逻辑有疑问------

void MoGong::ChongNeng()
{
     state=CHONG_NENG;
     gui->reset();
     Player* myself = dataInterface->getMyself();

     tipArea->setMsg(QStringLiteral("弃到4张手牌，并选择摸牌数量"));
     for(int i = 1; i < 5; i ++)
         tipArea->addBoxItem(QString::number(i));
     tipArea->showBox();

     if(myself->getHandCardNum()<=4)
         decisionArea->enable(0);
     else
     {
         decisionArea->disable(0);
         handArea->enableAll();
         handArea->setQuota(myself->getHandCardNum()-4);
     }
     decisionArea->enable(1);
}

void MoGong::MoYan()
{
     state=MO_YAN;
     gui->reset();
     tipArea->setMsg(QStringLiteral("【选择目标角色】弃1张牌或【不选择目标】你摸3张牌"));
     playerArea->enableAll();
     playerArea->setQuota(1);
     decisionArea->enable(0);
     decisionArea->enable(1);
}

void MoGong::ChongNengGaiPai()
{
    state=CHONG_NENG_GAI_PAI;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    QList<Card*> covercards=dataInterface->getCoverCards();

    tipArea->setMsg(QStringLiteral("请选择最多")+QString::number(chongnengNum)+QStringLiteral("张手牌盖做充能"));

    tipArea->setMsg(QStringLiteral("选择手牌盖做充能"));
      handArea->enableAll();
      handArea->setQuota(0,chongnengNum);
    decisionArea->enable(0);
}

void MoGong::cardAnalyse()
{
    Role::cardAnalyse();
    switch(state)
    {
    case CHONG_NENG:
            decisionArea->enable(0);
        break;
    case CHONG_NENG_GAI_PAI:
        if(gui->getHandArea()->getSelectedCards().size()<gui->getHandArea()->getHandCardItems().size()) //???
            decisionArea->enable(0);
        else
            decisionArea->disable(0);
        break;
    case MO_YAN_GAI_PAI:
        decisionArea->enable(0);
        break;
    }
}

void MoGong::coverCardAnalyse()
{
    Role::coverCardAnalyse();
    QList<Card*> selectedCoverCards = this->coverArea->getSelectedCards();
    switch(state)
    {
    case MO_GUAN_CHONG_JI:
        if(selectedCoverCards[0]->getElement()=="fire")
            decisionArea->enable(0);
        break;
  //  case 2602:
      case MO_GUAN_CHONG_JI_HIT:
        decisionArea->enable(0);
        break;
  //  case 2603:
     case LEI_GUANG_SAN_SHE:
        if(selectedCoverCards.size()==1)
        {
            decisionArea->enable(0);
            playerArea->disableAll();
            playerArea->setQuota(0);
        }
        else
        {
            decisionArea->disable(0);
            playerArea->enableEnemy();
            playerArea->setQuota(1);
        }
        break;
  // case 2605:
     case DUO_CHONG_SHE_JI:
        setAttackTarget();
        playerArea->disablePlayerItem(lastTarget);
        break;
    }
}

void MoGong::onOkClicked()
{

    QString text;
    QList<Card*> selectedCards = handArea->getSelectedCards();
    QList<Card*> selectedCoverCards = coverArea->getSelectedCards();
    QList<Player*> selectedPlayers = playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;

    switch(state)
    {
//NORMALACTION
    case 1:
//追加行动
    case 10:
    case 11:
    case 12:
        if(selectedCards.size()>0&&selectedCards[0]->getType()=="attack")
            lastTarget = selectedPlayers[0]->getID();
        break;

      case MO_GUAN_CHONG_JI:
        respond = newRespond(MO_GUAN_CHONG_JI);
      //  respond->add_args(selectedCoverCards[0]->getID());
        respond->add_args(1);
        respond->add_card_ids(selectedCoverCards[0]->getID());
        MoGuanChongJiUsed=true;
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
       case MO_GUAN_CHONG_JI_HIT:
        respond = newRespond(MO_GUAN_CHONG_JI_HIT);
        respond->add_args(1);
        respond->add_card_ids(selectedCoverCards[0]->getID());
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
      case LEI_GUANG_SAN_SHE:
        action = newAction(ACTION_MAGIC_SKILL,LEI_GUANG_SAN_SHE);
        if(selectedPlayers.size()!=0)
            action->add_dst_ids(selectedPlayers[0]->getID());
        foreach(Card*ptr,selectedCoverCards)
             action->add_card_ids(ptr->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
      case DUO_CHONG_SHE_JI:
        action = newAction(network::ACTION_ATTACK_SKILL,DUO_CHONG_SHE_JI); //攻击行动
        action->add_card_ids(selectedCoverCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());
        lastTarget = selectedPlayers[0]->getID();
        DuoChongSheJiUsed = true;
        emit sendCommand(network::MSG_ACTION, action);
         gui->reset();
        break;
      case CHONG_NENG_MO_YAN:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(CHONG_NENG_MO_YAN);
        text = tipArea->getBoxCurrentText();
        if(text[0]=='1')  
            respond->add_args(1);    
        else
            respond->add_args(2);
        gui->reset();
        emit sendCommand(network::MSG_RESPOND, respond);
        break;
    case CHONG_NENG:
        respond = new Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(CHONG_NENG);
        respond->add_args(1);                                              //参数1:1表示充能
        respond->add_args(selectedCards.size());                           //参数2：弃牌的张数
        foreach(Card*ptr,selectedCards){
            respond->add_card_ids(ptr->getID());
        }
        respond->add_args(tipArea->getBoxCurrentText()[0].digitValue());  //参数3：摸牌的张数
        ChongNengUsed=true;                                               //为【雷光散射】按钮服务
        chongnengNum=tipArea->getBoxCurrentText()[0].digitValue();
        emit sendCommand(network::MSG_RESPOND, respond);
        start=true;
        gui->reset();
        break;
    case MO_YAN:
        respond = new Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(MO_YAN);
        respond->add_args(1);                                             //参数1:1表示魔眼
        respond->add_args(selectedPlayers.size());                        //参数2：选择的目标个数
        if(selectedPlayers.size()>0)
            respond->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_RESPOND, respond);
        start=true;
        gui->reset();
        break;

      case CHONG_NENG_GAI_PAI:
        respond = newRespond(CHONG_NENG_GAI_PAI);
        respond->add_args(1);                       //参数1:1
        respond->add_args(selectedCards.size());
       foreach(Card*ptr,selectedCards){
            respond->add_card_ids(ptr->getID());
        }
 //       respond->add_args(tipArea->getBoxCurrentText()[0].digitValue());  //参数2：摸牌的张数
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

    }
    Role::onOkClicked();
}

void MoGong::onCancelClicked()
{
    Role::onCancelClicked();

    network::Respond* respond;
    switch(state)
    {
    case MO_GUAN_CHONG_JI:
        respond = newRespond(MO_GUAN_CHONG_JI);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case LEI_GUANG_SAN_SHE:
        normal();
        break;
      case MO_GUAN_CHONG_JI_HIT:
        respond = newRespond(MO_GUAN_CHONG_JI_HIT);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
      case  DUO_CHONG_SHE_JI:
          gui->reset();
        break;
    case CHONG_NENG_MO_YAN:
    case CHONG_NENG:
    case MO_YAN:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(CHONG_NENG_MO_YAN);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case CHONG_NENG_GAI_PAI:
      respond = newRespond(CHONG_NENG_GAI_PAI);
      respond->add_args(0);
      emit sendCommand(network::MSG_RESPOND, respond);
      gui->reset();
      break;
    }
}

void MoGong::attackAction()
{
    //若是连续技的额外行动，则只能用风系
    if(DUO_CHONG_SHE_JI == chosenAction){
          DuoChongSheJi();
    }
    else{
        Role::attackAction();
    }
}

void MoGong::turnBegin()
{
    Role::turnBegin();
    MoGuanChongJiUsed = false;
    DuoChongSheJiUsed = false;
    ChongNengUsed = false;
    lastTarget=-1;
}

void MoGong::askForSkill(network::Command* cmd)
{
     //灵魂链接稍后补上
    if(cmd->respond_id() == CHONG_NENG_MO_YAN)
         ChongNengMoYan();
    else if(cmd->respond_id() == CHONG_NENG)
           ChongNeng();
    else if(cmd->respond_id() == MO_YAN)
           MoYan();
    else if(cmd->respond_id() == MO_GUAN_CHONG_JI_HIT)
         MoGuanChongJiHit();
    else if(cmd->respond_id() == CHONG_NENG_GAI_PAI)
         ChongNengGaiPai();
    else if(cmd->respond_id() == MO_GUAN_CHONG_JI)
        MoGuanChongJi();
    else
        Role::askForSkill(cmd);
}

