#include "DieWu.h"

enum CAUSE{
    SHENG_MING_ZHI_HUO=2401,
    WU_DONG=2402,
    WU_DONG_EXTRA=24021,
    DU_FEN=2403,
    CHAO_SHENG=2404,
    JING_HUA_SHUI_YUE=2405,
    DIAO_LING=2406,
    DIAO_LING_EXTRA=24061,
    YONG_HUA=2407,
    DAO_NI_ZHI_DIE=2408,
    DAO_NI_ZHI_DIE_EXTRA=24081
};

DieWu::DieWu()
{
    makeConnection();
    setMyRole(this);
    Button *wuDong, *yongHua, *daoNiZhiDie, *checkCover;
    wuDong = new Button(3,QStringLiteral("舞动"));
    buttonArea->addButton(wuDong);
    connect(wuDong,SIGNAL(buttonSelected(int)),this,SLOT(WuDong1()));

    yongHua = new Button(4,QStringLiteral("蛹化"));
    buttonArea->addButton(yongHua);
    connect(yongHua,SIGNAL(buttonSelected(int)),this,SLOT(YongHua()));

    daoNiZhiDie = new Button(5,QStringLiteral("倒逆之蝶"));
    buttonArea->addButton(daoNiZhiDie);
    connect(daoNiZhiDie,SIGNAL(buttonSelected(int)),this,SLOT(DaoNiZhiDie1()));

    checkCover = new Button(10,QStringLiteral("查看茧"));
    buttonArea->addOutsideTurnButton(checkCover);
    checkCover->setVisible(true);
    checkCover->setEnabled(true);
    connect(checkCover,SIGNAL(buttonSelected(int)),gui,SLOT(showCoverArea()));
    connect(checkCover,SIGNAL(buttonUnselected(int)),gui,SLOT(closeCoverArea()));
}

void DieWu::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    buttonArea->enable(3);
    if(myself->getGem()>0)
        buttonArea->enable(4);
    if(myself->getEnergy()>0)
        buttonArea->enable(5);
    unactionalCheck();
}

void DieWu::WuDong1()
{
    state=WU_DONG;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->enable(0);

        tipArea->setMsg(QStringLiteral("请先选择一项："));
        tipArea->addBoxItem(QStringLiteral("1.摸1张牌【强制】"));
    if(dataInterface->getHandCards().size()>0)
        tipArea->addBoxItem(QStringLiteral("2.弃1张牌【强制】"));
    tipArea->showBox();
}

void DieWu::WuDong2()
{
    state=WU_DONG_EXTRA;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    handArea->setQuota(1);
    handArea->enableAll();

    decisionArea->enable(1);
}

void DieWu::DuFen()
{
    state=DU_FEN;
    tipArea->setMsg(QStringLiteral("是否发动毒粉？"));

    gui->showCoverArea(true);
    HandArea *coverArea = gui->getCoverArea();
    coverArea->reset();
    coverArea->enableAll();
    coverArea->setQuota(1);
    decisionArea->enable(1);
}

void DieWu::ChaoSheng()
{
    state=CHAO_SHENG;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    tipArea->setMsg(QStringLiteral("是否发动朝圣？"));

    gui->showCoverArea(true);
    coverArea->reset();
    coverArea->enableAll();
    coverArea->setQuota(1);
    decisionArea->enable(1);
}

void DieWu::JingHuaShuiYue()
{

    state= JING_HUA_SHUI_YUE;
    tipArea->setMsg(QStringLiteral("是否发动镜花水月？"));
    gui->showCoverArea(true);
    HandArea *coverArea = gui->getCoverArea();
    coverArea->reset();
    coverArea->enableAll();
    coverArea->setQuota(2);
    decisionArea->enable(1);
}

void DieWu::DiaoLing(int type)
{
    state=DIAO_LING;
    if(type == 2)
    {
        tipArea->setMsg(QStringLiteral("是否发动凋零？"));
        playerArea->enableAll();
        playerArea->setQuota(1);
    }
    else
        tipArea->setMsg(QStringLiteral("非法术牌不能发动凋零"));

    decisionArea->enable(1);
}

void DieWu::YongHua()
{
    state=YONG_HUA;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void DieWu::DaoNiZhiDie1()
{
 // state = 2471;
    state=DAO_NI_ZHI_DIE;
    handArea->reset();
    playerArea->reset();
    decisionArea->reset();
    tipArea->reset();
    tipArea->setMsg(QStringLiteral("你弃2张牌，再选择一项："));
    tipArea->addBoxItem(QStringLiteral("1.对目标角色造成1点法术伤害，该伤害不能用治疗抵御"));
    if(dataInterface->getMyself()->getToken(2)>=2)
        tipArea->addBoxItem(QStringLiteral("2.（移除2个【茧】）移除1个【蛹】"));
    tipArea->addBoxItem(QStringLiteral("3.（自己造成4点法术伤害③）移除1个【蛹】"));
    tipArea->showBox();
    QList<Card*> handcards=dataInterface->getHandCards();
    if(handcards.size()>1)
        handArea->setQuota(2);
    else if(handcards.size()==1)
        handArea->setQuota(1);
    handArea->enableAll();
    decisionArea->enable(1);
    if(handcards.size()==0)
        decisionArea->enable(0);
}

void DieWu::DaoNiZhiDie2()
{
  //  state = 2472;
    state=DAO_NI_ZHI_DIE_EXTRA;
    playerArea->reset();
    tipArea->reset();

    switch(daoNiZhiDieFlag)
    {
    case 1:
        playerArea->enableAll();
        playerArea->setQuota(1);
        break;
    case 2:
        gui->showCoverArea(true);
        coverArea->reset();
        coverArea->enableAll();
        coverArea->setQuota(2);
        break;
    }
    decisionArea->disable(0);
}

void DieWu::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*> selectedCards = handArea->getSelectedCards();
    QList<Card*> selectedCoverCards = coverArea->getSelectedCards();
    QList<Player*> selectedPlayers = playerArea->getSelectedPlayers();

    network::Action *action;
    network::Respond *respond;

    switch(state)
    {
    case WU_DONG:
        if(tipArea->getBoxCurrentText()[0].digitValue()==1)
        {
            wudongmopai = true;
            action = newAction(ACTION_MAGIC_SKILL,WU_DONG);
            action->set_src_id(myID);
            action->set_action_id(WU_DONG);
            action->add_args(1);
            emit sendCommand(network::MSG_ACTION, action);
            gui->reset();
        }
        else
        {
            wudongmopai = false;
            WuDong2();
        }
        break;

      case WU_DONG_EXTRA:
        action = newAction(ACTION_MAGIC_SKILL,WU_DONG);
        action->set_src_id(myID);
        action->set_action_id(WU_DONG);
        action->add_args(2);
        action->add_card_ids(selectedCards[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
 //   case 2402:
     case DU_FEN:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(DU_FEN);
        respond->add_args(1);
        respond->add_card_ids(selectedCoverCards[0]->getID());
        coverArea->reset();
        gui->showCoverArea(false);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

      case CHAO_SHENG:
        respond = newRespond(CHAO_SHENG);
        respond->set_src_id(myID);
        respond->add_args(1);
        respond->add_args(selectedCoverCards[0]->getID());
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

    case  JING_HUA_SHUI_YUE:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->add_args(1);
        respond->set_respond_id(JING_HUA_SHUI_YUE);
        respond->add_card_ids(selectedCoverCards[0]->getID());
        respond->add_card_ids(selectedCoverCards[1]->getID());
        coverArea->reset();
        gui->showCoverArea(false);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

     case DIAO_LING:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(DIAO_LING);
        respond->add_args(1);
        respond->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

     case YONG_HUA:
        action = newAction(ACTION_MAGIC_SKILL,YONG_HUA);
        action->set_src_id(myID);
        action->set_action_id(YONG_HUA);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case DAO_NI_ZHI_DIE:
        daoNiZhiDieFlag = tipArea->getBoxCurrentText()[0].digitValue();
        if(daoNiZhiDieFlag == 3)
        {
            action = newAction(ACTION_MAGIC_SKILL,DAO_NI_ZHI_DIE);
            action->set_src_id(myID);
            action->set_action_id(DAO_NI_ZHI_DIE);
            action->add_args(daoNiZhiDieFlag);

            foreach(Card*ptr,selectedCards){
                action->add_card_ids(ptr->getID());
            }
            emit sendCommand(network::MSG_ACTION, action);
            gui->reset();
        }
        else
            DaoNiZhiDie2();
        break;
    case DAO_NI_ZHI_DIE_EXTRA:

        action = newAction(ACTION_MAGIC_SKILL,DAO_NI_ZHI_DIE);
        action->set_src_id(myID);
        action->set_action_id(DAO_NI_ZHI_DIE);
        action->add_args(daoNiZhiDieFlag);

        foreach(Card*ptr,selectedCards){
            action->add_card_ids(ptr->getID());
        }

        if(daoNiZhiDieFlag==1)
        {
            action->add_dst_ids(selectedPlayers[0]->getID());
        }
        else
        {
            foreach(Card*ptr,selectedCoverCards){
                action->add_args(ptr->getID());
            }

            coverArea->reset();
            gui->showCoverArea(false);
        }
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();;
        break;
    }
}

void DieWu::onCancelClicked()
{
    Role::onCancelClicked();

    network::Respond *respond;

    switch(state)
    {
    case WU_DONG:
    case YONG_HUA:
    case DAO_NI_ZHI_DIE:
        normal();
        break;
    case DU_FEN:
        respond = newRespond(DU_FEN);
        respond->add_args(0);
        coverArea->reset();
        gui->showCoverArea(false);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
      case CHAO_SHENG:
        respond = newRespond(CHAO_SHENG);
        respond->add_args(0);
        coverArea->reset();
        gui->showCoverArea(false);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
      case  JING_HUA_SHUI_YUE:
        respond = newRespond(JING_HUA_SHUI_YUE);
        respond->set_src_id(myID);
        respond->add_args(0);
        coverArea->reset();
        gui->showCoverArea(false);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
     case DIAO_LING:
        respond = newRespond(DIAO_LING);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
      case WU_DONG_EXTRA:
        WuDong1();
        break;
      case DAO_NI_ZHI_DIE_EXTRA:
        DaoNiZhiDie1();
        coverArea->reset();
        gui->showCoverArea(false);
        break;
    }
}

void DieWu::cardAnalyse()
{
    Role::cardAnalyse();
    switch(state)
    {
    case WU_DONG_EXTRA:
        decisionArea->enable(0);
        break;
    case DAO_NI_ZHI_DIE:
        decisionArea->enable(0);
        break;
    }
}

void DieWu::coverCardAnalyse()
{

    Role::coverCardAnalyse();
    QList<Card*> selectedCoverCards = this->coverArea->getSelectedCards();

   switch(state)
    {
    case DU_FEN:
    case CHAO_SHENG:
    case DAO_NI_ZHI_DIE_EXTRA:
        decisionArea->enable(0);
        break;

    case JING_HUA_SHUI_YUE:
        if(selectedCoverCards[0]->getElement()!=selectedCoverCards[1]->getElement())
            decisionArea->disable(0);
        else
            decisionArea->enable(0);
        break;
    }
}


void DieWu::askForSkill(Command* cmd)
{
    switch(cmd->respond_id())
    {

    case DU_FEN:
        DuFen();
        break;
    case CHAO_SHENG:
        ChaoSheng();
        break;
    case JING_HUA_SHUI_YUE:
        JingHuaShuiYue();
        break;
    case DIAO_LING:
        DiaoLing(cmd->args(0));
        break;
    default:
        Role::askForSkill(cmd);
    }
}

