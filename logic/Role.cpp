#include "Role.h"
#include <QStringList>
#ifdef SOUND
  #include <QSound>
  #define QSound(x) QSound::play(x)
#else
  #define QSound(x)
#endif

#include "data/Common.h"
#include "data/DataInterface.h"
#include "widget/GUI.h"
#include "logic/Logic.h"
#define POLLING 61
Role::Role(QObject *parent) :
    QObject(parent)
{
    Player*myself=dataInterface->getMyself();
    myID = dataInterface->getID();
    int myColor=myself->getColor();

    network::GameInfo* gameInfo=dataInterface->getGameInfo();
    int playerMax=dataInterface->getPlayerMax();
    red=dataInterface->getRedTeam();
    blue=dataInterface->getBlueTeam();
    playerList=dataInterface->getPlayerList();
    if(myID != GUEST){
        int i;
        //find myPos
        for(i=0;i<playerMax;i++)
            if(gameInfo->player_infos(i).id()==myID)
                break;
        int ptr;
        do
        {
            i++;
            if(i == playerMax)
                i=0;
            ptr = gameInfo->player_infos(i).id();

        }while(playerList[ptr]->getColor() == myColor);
        nextCounterClockwise = ptr;
    }
    handArea = gui->getHandArea();
    playerArea = gui->getPlayerArea();
    buttonArea = gui->getButtonArea();
    decisionArea = gui->getDecisionArea();
    tipArea = gui->getTipArea();
    teamArea = gui->getTeamArea();
    coverArea = gui->getCoverArea();    
    showArea = gui->getShowArea();
    timelineBar = gui->getTimelineBar();

}
void Role::makeConnection()
{
    connect(logic->getClient(),SIGNAL(getMessage(unsigned short, google::protobuf::Message*)),
            this,SLOT(decipher(unsigned short, google::protobuf::Message*)));

    connect(this,SIGNAL(sendCommand(unsigned short, google::protobuf::Message*)),logic->getClient(),SLOT(sendMessage(unsigned short, google::protobuf::Message*)));
    connect(decisionArea,SIGNAL(okClicked()),this,SLOT(onOkClicked()));
    connect(decisionArea,SIGNAL(cancelClicked()),this,SLOT(onCancelClicked()));
    connect(decisionArea,SIGNAL(exchangeClicked()),this,SLOT(exchangeCards()));
    connect(decisionArea,SIGNAL(resignClicked()),this,SLOT(resign()));
    connect(buttonArea->getButtons().at(0),SIGNAL(buttonSelected(int)),this,SLOT(buy()));
    connect(buttonArea->getButtons().at(1),SIGNAL(buttonSelected(int)),this,SLOT(synthetize()));
    connect(buttonArea->getButtons().at(2),SIGNAL(buttonSelected(int)),this,SLOT(extract()));
    connect(buttonArea,SIGNAL(buttonUnselected()),this,SLOT(onCancelClicked()));
    connect(handArea,SIGNAL(cardReady()),this,SLOT(cardAnalyse()));
    connect(coverArea,SIGNAL(cardReady()),this,SLOT(coverCardAnalyse()));
    connect(playerArea,SIGNAL(playerReady()),this,SLOT(playerAnalyse()));
}

void Role::coverCardAnalyse()
{
    switch(state)
    {
    case 50:
        decisionArea->enable(0);
        break;
    }
}

void Role::cardAnalyse()
{
    int i;
    SafeList<Card*> selectedCards=handArea->getSelectedCards();
    QString cardName;
    try{
    switch (state)
    {
//normal action
    case 1:
//追加攻击或法术行动
    case 11:
    case 12:
        if(selectedCards[0]->getType()=="magic" )
        {
            cardName=selectedCards[0]->getName();
            if(cardName==QStringLiteral("中毒"))
                playerArea->enableAll();
            if(cardName==QStringLiteral("虚弱"))
            {
                playerArea->reset();
                for(i=0;i<dataInterface->getPlayerMax();i++)
                    if(!dataInterface->getPlayerList().at(i)->checkBasicStatus(1))
                        playerArea->enablePlayerItem(i);
            }
            if(cardName==QStringLiteral("圣盾"))
            {
                playerArea->reset();
                for(i=0;i<dataInterface->getPlayerMax();i++)
                    if(!dataInterface->getPlayerList().at(i)->checkBasicStatus(2))
                        playerArea->enablePlayerItem(i);
            }
            if(cardName==QStringLiteral("魔弹"))
            {
                playerArea->reset();
                playerArea->enablePlayerItem(nextCounterClockwise);
            }
        }
        else
    case 10:
            setAttackTarget();
    break;
//attacked reply
    case 2:
        if(selectedCards[0]->getType()=="attack" )
        {
            playerArea->reset();
            playerArea->enableEnemy();
            playerArea->disablePlayerItem(sourceID);
        }
        if(selectedCards[0]->getElement()=="light")
        {
            playerArea->reset();
            decisionArea->enable(0);
        }
    break;
//drop cards
    case 3:
//天使祝福给牌
    case 751:
//魔爆冲击弃牌,充盈
    case 851:
    case 2951:
        decisionArea->enable(0);
    break;
//魔弹reply
    case 8:
        cardName=selectedCards[0]->getName();
        if(cardName==QStringLiteral("魔弹"))
        {
            playerArea->reset();
            playerArea->enablePlayerItem(moDanNextID);
        }
        else if(cardName==QStringLiteral("圣光"))
        {
            playerArea->reset();
            decisionArea->enable(0);
        }
        break;
    }

    }catch(int error){
        logic->onError(error);
    }
}

void Role::playerAnalyse()
{
    decisionArea->enable(0);
}

void Role::setAttackTarget()
{
    playerArea->reset();
    playerArea->enableEnemy();
//潜行
    for(int i=0;i<playerList.size();i++)
        if(playerList[i]->getRoleID()==5 && playerList[i]->getTap()==1){
            playerArea->disablePlayerItem(i);
            break;
        }
//挑衅
    if(playerList[myID]->getSpecial(1) == 1)
        for(int i=0;i<playerList.size();i++)
            if(playerList[i]->getRoleID()!=21)
                playerArea->disablePlayerItem(i);

}

void Role::exchangeCards()
{
    network::Action* action = newAction(ACTION_UNACTIONAL);
    emit sendCommand(network::MSG_ACTION, action);
}

void Role::resign()
{
    if(state == 42){
        network::Respond *respond = newRespond(RESPOND_ADDITIONAL_ACTION);
        respond->add_args(ACTION_NONE);
        emit sendCommand(network::MSG_RESPOND, respond);
    }
    else{
        network::Action* action = newAction(ACTION_NONE);
        emit sendCommand(network::MSG_ACTION, action);
    }
    gui->reset();
}

//正常行动
void Role::normal()
{
    gui->reset();

    SafeList<Card*> handcards=dataInterface->getHandCards();
    Player* myself=dataInterface->getMyself();
    Team* myTeam=dataInterface->getMyTeam();
    int n=handcards.count();

    state=1;
    actionFlag=0;
    playerArea->setQuota(1);
    handArea->setQuota(1);

    handArea->enableAll();
    handArea->disableElement("light");

    if(n+3 <= myself->getHandCardMax() && start==false)
    {
//购买
        buttonArea->enable(0);
        if(myTeam->getEnergy()>=3)
//合成
            buttonArea->enable(1);
    }
//提炼
    if(myself->getEnergy()<myself->getEnergyMax() && myTeam->getEnergy()>0 && start==false)
        buttonArea->enable(2);
    tipArea->setMsg(QStringLiteral("轮到你了"));
    unactionalCheck();
	

}

//无法行动检测
void Role::unactionalCheck()
{
    bool actional=false;
    SafeList<Button*>buttons=buttonArea->getButtons();
    SafeList<CardItem*>cardItems=handArea->getHandCardItems();
    for(int i=0;i<buttons.size();i++)
        if(buttons[i]->isEnabled()&&i!=2)
        {
            actional=true;
            break;
        }
    for(int i=0;i<cardItems.size()&&!actional;i++)
        if(cardItems[i]->isEnabled())
        {
            actional=true;
            break;
        }
    if(!actional)
        decisionArea->enable(2);
    else
        decisionArea->disable(2);
}

void Role::attackAction()
{
    gui->reset();
	
	//[YongZhe]挑衅 
	if(playerList[myID]->getSpecial(1) == 1){
		tipArea->setMsg(QStringLiteral("你被勇者挑衅了"));		
	}
	
    state=10;
    actionFlag=1;
    playerArea->setQuota(1);
    handArea->setQuota(1);
    handArea->enableAttack();
    decisionArea->enable(3);
}

void Role::magicAction()
{
    gui->reset();

    state=11;
    actionFlag=2;
    playerArea->setQuota(1);
    handArea->setQuota(1);
    handArea->enableMagic();
    handArea->disableElement("light");
    decisionArea->enable(3);

}

void Role::attackOrMagic()
{
    gui->reset();

    state=12;
    actionFlag=4;
    playerArea->setQuota(1);
    handArea->setQuota(1);
    handArea->enableAll();
    handArea->disableElement("light");
    decisionArea->enable(3);
}

void Role::attacked(QString element,int hitRate)
{
    state=2;
    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    if(hitRate==0)
    {
        handArea->enableElement(element);
        handArea->enableElement("darkness");
    }
    handArea->disableMagic();
    handArea->enableElement("light");
    gui->alert();
}
//add 充盈弃牌提示
void Role::drop(int howMany, int cause)
{
    state=3;
    handArea->setQuota(howMany);
    switch(cause)
    {
    case MO_BAO_CHONG_JI:
        handArea->enableMagic();
        tipArea->setMsg(QStringLiteral("弃一张法牌，否则受两点法术伤害"));
        decisionArea->enable(1);
        break;
    case SP_MO_BAO_CHONG_JI:
        handArea->enableMagic();
        tipArea->setMsg(QStringLiteral("弃一张法牌，否则受两点法术伤害"));
        decisionArea->enable(1);
        break;
    case CHONG_YING_DISCARD:
        handArea->enableAll();
        tipArea->setMsg(QStringLiteral("弃1张牌，魔枪方角色可不如此做【除魔枪外每弃1张雷系牌或法术牌，魔枪下次的攻击伤害+1】"));
        decisionArea->enable(1);
        break;
    case CHONG_YING:
        handArea->enableAll();
        tipArea->setMsg(QStringLiteral("弃1张牌，【每弃1张雷系牌或法术牌，魔枪下次的攻击伤害+1】"));
        break;
    default:
        handArea->enableAll();
        tipArea->setMsg(QStringLiteral("你需要弃")+QString::number(howMany)+QStringLiteral("张牌"));
    }
    gui->alert();
}

void Role::dropCover(int howMany, int cause)
{
    state = 50;
    gui->showCoverArea(true);
    HandArea *coverArea = gui->getCoverArea();
    coverArea->reset();
    coverArea->setQuota(howMany);
    coverArea->enableAll();
    if(cause == CAUSE_OVERLOAD){
        tipArea->setMsg(QStringLiteral("你需要弃")+QString::number(howMany)+QStringLiteral("张盖牌"));
    }
    gui->alert();
}

void Role::buy()
{
    Team*team=dataInterface->getMyTeam();

    int energy=team->getEnergy();

    state=4;
    decisionArea->enable(0);
    decisionArea->enable(1);
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    if(energy<4)
        tipArea->setMsg(QStringLiteral("你摸3张牌，你方战绩区加一宝石一水晶"));
    if(energy==4)
    {
        tipArea->setMsg(QStringLiteral("战绩区星石已有4个，请选择要购买的星石："));
        tipArea->addBoxItem(QStringLiteral("1.一个宝石"));
        tipArea->addBoxItem(QStringLiteral("2.一个水晶"));
        tipArea->showBox();
    }
    if(energy==5)
    {
        tipArea->setMsg(QStringLiteral("战绩区星石数目已达上限，购买将不再增加星石"));
    }
}

void Role::synthetize()
{
    int gem,crystal;
    Team* team;

    team=dataInterface->getMyTeam();
    gem=team->getGem();
    crystal=team->getCrystal();

    state=5;
    decisionArea->enable(0);
    decisionArea->enable(1);
    tipArea->reset();
    handArea->reset();
    playerArea->reset();

    tipArea->setMsg(QStringLiteral("请选择用来合成的星石："));
    if(crystal>=3)
        tipArea->addBoxItem(QStringLiteral("1.三个水晶"));
    if(crystal>=2&&gem>=1)
        tipArea->addBoxItem(QStringLiteral("2.两个水晶和一个宝石"));
    if(crystal>=1&&gem>=2)
        tipArea->addBoxItem(QStringLiteral("3.一个水晶和两个宝石"));
    if(gem>=3)
        tipArea->addBoxItem(QStringLiteral("4.三个宝石"));
    tipArea->showBox();
}

void Role::extract()
{
    int gem,crystal;
    Team* team;
    Player* myself=dataInterface->getPlayerList().at(myID);

    team=dataInterface->getMyTeam();
    gem=team->getGem();
    crystal=team->getCrystal();

    state=6;
    decisionArea->enable(0);
    decisionArea->enable(1);
    tipArea->reset();
    handArea->reset();
    playerArea->reset();

    tipArea->setMsg(QStringLiteral("请选择要提取的星石数："));
    switch(myself->getEnergyMax()-myself->getEnergy())
    {
    case 4:
    case 3:
    case 2:
        if(gem>=2)
            tipArea->addBoxItem(QStringLiteral("1.两个宝石"));
        if(crystal>=2)
            tipArea->addBoxItem(QStringLiteral("2.两个水晶"));
        if(gem>=1&&crystal>=1)
            tipArea->addBoxItem(QStringLiteral("3.一个宝石和一个水晶"));
    case 1:
        if(gem>=1)
            tipArea->addBoxItem(QStringLiteral("4.一个宝石"));
        if(crystal>=1)
            tipArea->addBoxItem(QStringLiteral("5.一个水晶"));
    }
    tipArea->showBox();
}

void Role::moDaned(int nextID,int sourceID,int howMany)
{
    state=8;
    gui->reset();
    QString msg=dataInterface->getPlayerList().at(sourceID)->getRoleName()+QStringLiteral("对")+QStringLiteral("你")+QStringLiteral("使用了魔弹，目前伤害为：")+QString::number(howMany)+QStringLiteral("点");
    tipArea->setMsg(msg);
    playerArea->setQuota(1);
    handArea->setQuota(1);

    handArea->enableElement("light");
    handArea->enableMoDan();
    decisionArea->enable(1);
    moDanNextID=nextID;
    gui->alert();
}

void Role::cure(int cross,int harmPoint, int type, int crossAvailable)
{
    int min=crossAvailable<harmPoint?crossAvailable:harmPoint;
    QString msg=QStringLiteral("你受到")+QString::number(harmPoint)+QStringLiteral("点");
    if(type==1)
        msg+=QStringLiteral("攻击");
    else
        msg+=QStringLiteral("法术");
    msg+=QStringLiteral("伤害，要使用多少个治疗抵御之？");

    state=9;
    decisionArea->enable(0);
    decisionArea->enable(1);

    tipArea->setMsg(msg);
    for(;min>=0;min--)
        tipArea->addBoxItem(QString::number(min));
    tipArea->showBox();
    gui->alert();
}

void Role::turnBegin()
{
    isMyTurn=true;
    onceUsed=false;
    start=false;
    usedAttack=usedMagic=usedSpecial=false;
    gui->alert();
}

void Role::additionalAction(network::Command* cmd){
    gui->reset();
    state=42;
    tipArea->setMsg(QStringLiteral("是否执行额外行动？"));
    for(int i = 0; i < cmd->args_size(); i++){
        tipArea->addBoxItem(getCauseString(cmd->args(i)));
    }
    tipArea->showBox();
    decisionArea->enable(0);
    decisionArea->enable(3);
    gui->alert();
}

void Role::askForSkill(Command *cmd)
{
    int choices;
    switch (cmd->respond_id()) {
    case JI_ANG_KUANG_XIANG_QU:
        gui->reset();
        state = JI_ANG_KUANG_XIANG_QU;
        choices = cmd->args(0);
        if((choices&1)==1)
        {
            tipArea->addBoxItem(QStringLiteral("1.移除己方【战绩区】的2星石"));
        }
        if((choices&2)==2)
        {
            tipArea->addBoxItem(QStringLiteral("2.将【永恒乐章】转移给吟游诗人"));
        }
        tipArea->setMsg(QStringLiteral("选择【激昂狂想曲】发动条件"));
        tipArea->showBox();
        decisionArea->enable(0);
        decisionArea->enable(1);
        break;
    case JI_ANG_KUANG_XIANG_QU_2:
        gui->reset();
        state = JI_ANG_KUANG_XIANG_QU_2;
        tipArea->addBoxItem(QStringLiteral("1.对两名目标对手造成1点法术伤害③"));
        tipArea->addBoxItem(QStringLiteral("2.弃2张牌"));
        tipArea->setMsg(QStringLiteral("选择【激昂狂想曲】发动效果"));
        tipArea->showBox();
        decisionArea->enable(0);
        break;
    case SHENG_LI_JIAO_XIANG_SHI:
        gui->reset();
        state = SHENG_LI_JIAO_XIANG_SHI;
        choices = cmd->args(0);
        if((choices&1)==1)
        {
            tipArea->addBoxItem(QStringLiteral("1.对吟游诗人造成3点法术伤害③"));
        }
        if((choices&2)==2)
        {
            tipArea->addBoxItem(QStringLiteral("2.将【永恒乐章】转移给吟游诗人"));
        }
        tipArea->setMsg(QStringLiteral("选择【激昂狂想曲】发动条件或取消"));
        tipArea->showBox();
        decisionArea->enable(0);
        decisionArea->enable(1);
        break;
    case SHENG_LI_JIAO_XIANG_SHI_2:
        gui->reset();
        state = SHENG_LI_JIAO_XIANG_SHI_2;
        choices = cmd->args(0);
        if((choices&1)==1)
        {
            tipArea->addBoxItem(QStringLiteral("1.将己方【战绩区】的1星石提炼成为你的能量"));
        }
        if((choices&2)==2)
        {
            tipArea->addBoxItem(QStringLiteral("2.己方【战绩区】+1【宝石】，你+1【治疗】"));
        }
        tipArea->setMsg(QStringLiteral("选择【胜利交响诗】发动效果"));
        tipArea->showBox();
        decisionArea->enable(0);
        break;
    default:
        gui->reset();
        state=36;
        tipArea->setMsg(QStringLiteral("是否发动") + getCauseString(cmd->respond_id()) + "?");
        decisionArea->enable(0);
        decisionArea->enable(1);
        break;
    }

}

void Role::JiAngStone()
{
    state = JI_ANG_KUANG_XIANG_QU_STONE;
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

    decisionArea->enable(0);
}

void Role::JiAngHarm()
{
    gui->reset();
    state = JI_ANG_KUANG_XIANG_QU_HARM;

    playerArea->enableEnemy();
    playerArea->setQuota(2);

    decisionArea->enable(1);
}

void Role::ShengLiStone()
{
    state = SHENG_LI_JIAO_XIANG_SHI_STONE;
    gui->reset();
    Team* myTeam = dataInterface->getMyTeam();
    int gem, crystal;
    gem = myTeam->getGem();
    crystal = myTeam->getCrystal();
    tipArea->setMsg(QStringLiteral("请选择要提炼的星石"));
    if(crystal>=1)
        tipArea->addBoxItem(QStringLiteral("1.一个水晶"));
    if(gem>=1)
        tipArea->addBoxItem(QStringLiteral("2.一个宝石"));
    tipArea->showBox();

    decisionArea->enable(0);
}

void Role::onCancelClicked()
{
    network::Respond* respond;
    switch(state)
    {
//normal
    case 1:
    case 4:
    case 5:
    case 6:
        gui->reset();
        myRole->normal();
        break;
//ATTACKEDREPLY
    case 2:
        respond = newRespond(network::RESPOND_REPLY_ATTACK);
        respond->add_args(RA_GIVEUP);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//DROPREPLY
    case 3:
        respond = newRespond(network::RESPOND_DISCARD);
        respond->add_args(0);
        gui->reset();
        emit sendCommand(network::MSG_RESPOND, respond);
        break;
//虚弱
    case 7:
        respond = newRespond(network::RESPOND_WEAKEN);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//魔弹应答
    case 8:
        respond = newRespond(network::RESPOND_BULLET);
        respond->add_args(RA_GIVEUP);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//治疗应答
    case 9:
        respond = newRespond(network::RESPOND_HEAL);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//简单的技能发动询问
    case 36:
        respond = newRespond(skillCmd.respond_id());
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//弃盖牌
    case 50:
        respond = newRespond(RESPOND_DISCARD_COVER);
        respond->add_args(0);
        gui->reset();
        emit sendCommand(network::MSG_RESPOND, respond);
        break;

    case JI_ANG_KUANG_XIANG_QU:
        respond = newRespond(JI_ANG_KUANG_XIANG_QU);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case SHENG_LI_JIAO_XIANG_SHI:
        respond = newRespond(SHENG_LI_JIAO_XIANG_SHI);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
}

void Role::onOkClicked()
{
    SafeList<Card*>selectedCards;
    SafeList<Player*>selectedPlayers;

    int i,boxCurrentIndex,choice;
    int gem, crystal;

    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;
    try{
    switch(state)
    {
//NORMALACTION
    case 1:
//追加行动
    case 10:
    case 11:
    case 12:
        if(selectedCards[0]->getType()=="attack"){
            action = newAction(network::ACTION_ATTACK);
            action->add_card_ids(selectedCards[0]->getID());
            action->add_dst_ids(selectedPlayers[0]->getID());
            usedAttack=true;
            usedMagic=usedSpecial=false;
        }
        else
        {
            QString cardName;
            cardName=selectedCards[0]->getName();
            if(cardName==QStringLiteral("中毒")||cardName==QStringLiteral("虚弱")||cardName==QStringLiteral("圣盾")||cardName==QStringLiteral("魔弹"))
            {
                action = newAction(network::ACTION_MAGIC);
                action->add_card_ids(selectedCards[0]->getID());
                action->add_dst_ids(selectedPlayers[0]->getID());
            }
            usedMagic=true;
            usedAttack=usedSpecial=false;
        }
        gui->reset();
        emit sendCommand(network::MSG_ACTION, action);
        break;
//ATTACKEDREPLY
    case 2:
        respond = newRespond(network::RESPOND_REPLY_ATTACK);

        if(selectedCards[0]->getType()=="attack")
        {
            respond->add_args(RA_ATTACK);
            respond->add_card_ids(selectedCards[0]->getID());
            respond->add_dst_ids(selectedPlayers[0]->getID());
        }
        else if(selectedCards[0]->getElement()=="light")
        {
            respond->add_args(RA_BLOCK);
            respond->add_card_ids(selectedCards[0]->getID());
        }
        gui->reset();
        emit sendCommand(network::MSG_RESPOND, respond);
        break;
//DROPREPLY
    case 3:
        respond = newRespond(network::RESPOND_DISCARD);
        respond->add_args(1);

        for(i=0;i<selectedCards.count();i++)
        {
            respond->add_card_ids(selectedCards[i]->getID());
        }
        gui->reset();
        emit sendCommand(network::MSG_RESPOND, respond);
        break;
//BUY
    case 4:
        action = newAction(network::ACTION_SPECIAL);
        action->set_action_id(SPECIAL_BUY);
        boxCurrentIndex=tipArea->getBoxCurrentIndex();

        switch(boxCurrentIndex)
        {
        case -1:
            if(dataInterface->getMyTeam()->getEnergy()<4)
            {
                action->add_args(1);
                action->add_args(1);
            }
            else
            {
                action->add_args(0);
                action->add_args(0);
            }
            break;
        case 0:
            action->add_args(1);
            action->add_args(0);
            break;
        case 1:
            action->add_args(0);
            action->add_args(1);
            break;
        }
        gui->reset();
        usedSpecial=true;
        usedAttack=usedMagic=false;
        emit sendCommand(network::MSG_ACTION, action);
        break;
//SYNTHETIZE
    case 5:
        action = newAction(network::ACTION_SPECIAL);
        action->set_action_id(SPECIAL_SYNTHESIZE);
        text=tipArea->getBoxCurrentText();
        switch(text[0].digitValue())
        {
        case 1:
            action->add_args(0);
            action->add_args(3);
            break;
        case 2:
            action->add_args(1);
            action->add_args(2);
            break;
        case 3:
            action->add_args(2);
            action->add_args(1);
            break;
        case 4:
            action->add_args(3);
            action->add_args(0);
            break;
        }
        gui->reset();
        usedSpecial=true;
        usedAttack=usedMagic=false;
        emit sendCommand(network::MSG_ACTION, action);
        break;
//EXTRACT
    case 6:
        action = newAction(network::ACTION_SPECIAL);
        action->set_action_id(SPECIAL_EXTRACT);
        text=tipArea->getBoxCurrentText();
        switch(text[0].digitValue())
        {
        case 1:
            action->add_args(2);
            action->add_args(0);
            break;
        case 2:
            action->add_args(0);
            action->add_args(2);
            break;
        case 3:
            action->add_args(1);
            action->add_args(1);
            break;
        case 4:
            action->add_args(1);
            action->add_args(0);
            break;
        case 5:
            action->add_args(0);
            action->add_args(1);
            break;
        }
        gui->reset();
        usedSpecial=true;
        usedAttack=usedMagic=false;
        emit sendCommand(network::MSG_ACTION, action);
        break;
//虚弱
    case 7:
        respond = newRespond(network::RESPOND_WEAKEN);
        respond->add_args(1);
        gui->reset();
        emit sendCommand(network::MSG_RESPOND, respond);
        break;
//魔弹应答
    case 8:
        respond = newRespond(network::RESPOND_BULLET);
        if(selectedCards[0]->getName()==QStringLiteral("圣光"))
        {
            respond->add_args(RA_BLOCK);
            respond->add_card_ids(selectedCards[0]->getID());
        }
        else if(selectedCards[0]->getName()==QStringLiteral("魔弹"))
        {
            respond->add_args(RA_ATTACK);
            respond->add_card_ids(selectedCards[0]->getID());
            respond->add_dst_ids(selectedPlayers[0]->getID());
        }
        gui->reset();
        emit sendCommand(network::MSG_RESPOND, respond);
        break;
//治疗应答
    case 9:
        respond = newRespond(network::RESPOND_HEAL);
        text=tipArea->getBoxCurrentText();
        respond->add_args(text.toInt());
        gui->reset();
        emit sendCommand(network::MSG_RESPOND, respond);
        break;
//简单的技能发动询问
    case 36:
        respond = newRespond(skillCmd.respond_id());
        respond->add_args(1);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//额外行动
    case 42:
        respond = newRespond(RESPOND_ADDITIONAL_ACTION);
        chosenAction = skillCmd.args(tipArea->getBoxCurrentIndex());
        respond->add_args(chosenAction);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//弃盖牌
    case 50:
        respond = newRespond(RESPOND_DISCARD_COVER);
        respond->add_args(1);
        selectedCards = coverArea->getSelectedCards();
        for(i=0;i<selectedCards.count();i++)
        {
            respond->add_card_ids(selectedCards[i]->getID());
        }
        gui->reset();
        emit sendCommand(network::MSG_RESPOND, respond);
        break;

    case JI_ANG_KUANG_XIANG_QU:
        if(tipArea->getBoxCurrentText()[0].digitValue()==2)
        {
            respond = newRespond(JI_ANG_KUANG_XIANG_QU);
            respond->add_args(2);
            emit sendCommand(network::MSG_RESPOND, respond);
            gui->reset();
        }
        else
        {
            JiAngStone();
        }
        break;
    case JI_ANG_KUANG_XIANG_QU_STONE:
        respond = newRespond(JI_ANG_KUANG_XIANG_QU);
        respond->add_args(1);
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
    case JI_ANG_KUANG_XIANG_QU_2:
        if(tipArea->getBoxCurrentText()[0].digitValue()==1)
        {
            JiAngHarm();
        }
        else
        {
            respond = newRespond(JI_ANG_KUANG_XIANG_QU_2);
            respond->add_args(2);
            emit sendCommand(network::MSG_RESPOND, respond);
            gui->reset();
        }
        break;
    case JI_ANG_KUANG_XIANG_QU_HARM:
        respond = newRespond(JI_ANG_KUANG_XIANG_QU_2);
        respond->add_args(1);
        respond->add_dst_ids(selectedPlayers[0]->getID());
        respond->add_dst_ids(selectedPlayers[1]->getID());
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case SHENG_LI_JIAO_XIANG_SHI:
        respond = newRespond(SHENG_LI_JIAO_XIANG_SHI);
        respond->add_args(tipArea->getBoxCurrentText()[0].digitValue());
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case SHENG_LI_JIAO_XIANG_SHI_2:
        if(tipArea->getBoxCurrentText()[0].digitValue()==1)
        {
            ShengLiStone();
        }
        else
        {
            respond = newRespond(SHENG_LI_JIAO_XIANG_SHI_2);
            respond->add_args(2);
            emit sendCommand(network::MSG_RESPOND, respond);
            gui->reset();
        }
        break;
    case SHENG_LI_JIAO_XIANG_SHI_STONE:
        respond = newRespond(SHENG_LI_JIAO_XIANG_SHI_2);
        respond->add_args(1);
        respond->add_args(tipArea->getBoxCurrentText()[0].digitValue());
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case POLLING:
    {
        PollingResponse *res = new PollingResponse;
        TipArea *tipArea = gui->getTipArea();
        int chosen = tipArea->getBoxCurrentIndex();
        res->set_option(chosen);
        emit sendCommand(network::MSG_POLLING_REP, res);
        tipArea->reset();
        tipArea->setMsg(QStringLiteral("为避免误伤，等待期间不要离开"));
        break;
    }
    }
    }catch(int error){
        logic->onError(error);
    }
}

network::Action* Role::newAction(unsigned int action_type)
{
    network::Action* action = new network::Action();
    action->set_src_id(myID);
    action->set_action_type(action_type);
    return action;
}

network::Action* Role::newAction(unsigned int action_type, unsigned int action_id)
{
    network::Action* action = new network::Action();
    action->set_src_id(myID);
    action->set_action_type(action_type);
    action->set_action_id(action_id);
    return action;
}

network::Respond* Role::newRespond(unsigned int respond_id)
{
    network::Respond* respond = new network::Respond();
    respond->set_src_id(myID);
    respond->set_respond_id(respond_id);
    return respond;
}

void Role::decipher(quint16 proto_type, google::protobuf::Message* proto)
{
    int targetID;
    int cardID;
    int hitRate;
    int i,howMany;
    int cause;

    Card*card;
    Player*player;    
    SafeList<Card*> cards;
    QString msg;
    QString cardName;

    try{
    switch (proto_type)
    {
    case network::MSG_TURN_BEGIN:
        //回合开始
        targetID=((network::TurnBegin*)proto)->id();
        gui->logAppend("--------------------------------------");
        gui->logAppend(playerList[targetID]->getRoleName()+QStringLiteral("回合开始"));
        playerArea->setCurrentPlayerID(targetID);
        teamArea->setRound(((network::TurnBegin*)proto)->round());
        teamArea->update();

        if(targetID==dataInterface->getFirstPlayerID())
            teamArea->addRoundBy1();
        if(targetID!=myID)
        {
            isMyTurn=0;
            gui->reset();
            gui->setEnable(0);
        }
        else
        {
            myRole->turnBegin();
        }

        break;

    case network::MSG_CMD_REQ:
    {
        network::CommandRequest* cmd_req = (network::CommandRequest*)proto;
        for (int i = 0; i < cmd_req->commands_size(); ++i)
        {
            network::Command*cmd = (network::Command*)&(cmd_req->commands(i));
            switch (cmd->respond_id())
            {
            case network::RESPOND_REPLY_ATTACK:
                // 应战询问
                hitRate=cmd->args(0);
                cardID=cmd->args(1);
                targetID=cmd->args(2);
                sourceID=cmd->args(3);
                card=dataInterface->getCard(cardID);
                QSound("sound/Attack.wav");

                if(targetID!=myID)
                {
                    gui->setEnable(0);
                }
                else
                {
                    gui->setEnable(1);
                    msg=playerList[sourceID]->getRoleName()+QStringLiteral("对")+QStringLiteral("你")+QStringLiteral("使用了")+card->getName();
                    if (hitRate==2)
                        return;

                    if(hitRate==1)
                        msg+=QStringLiteral("，该攻击无法应战");

                    gui->reset();
                    tipArea->setMsg(msg);
                    myRole->attacked(card->getElement(),hitRate);
                }
                break;
            case network::RESPOND_DISCARD:
                //弃牌询问
                targetID = cmd->dst_ids(0);
                cause = cmd->args(0);
                howMany = cmd->args(1);
                gui->logAppend(getCommandString(cmd));
                if(targetID!=myID)
                {
                    gui->setEnable(0);
                }
                else
                {
                    gui->setEnable(1);
                    gui->reset();
                    myRole->drop(howMany, cause);
                }
                break;
            case network::RESPOND_DISCARD_COVER:
                //弃盖牌询问
                targetID = cmd->dst_ids(0);
                cause = cmd->args(0);
                howMany = cmd->args(1);
                gui->logAppend(getCommandString(cmd));
                if(targetID!=myID)
                {
                    gui->setEnable(0);
                }
                else
                {
                    gui->setEnable(1);
                    gui->reset();
                    dropCover(howMany, cause);
                }
                break;
            case network::RESPOND_WEAKEN:
                // 虚弱询问
                targetID=cmd->args(0);
                gui->logAppend(playerList[targetID]->getRoleName()+QStringLiteral("被虚弱了"));
                if(targetID!=myID)
                {
                    gui->setEnable(0);
                }
                else
                {
                    gui->setEnable(1);
                    gui->reset();
                    state=7;
                    decisionArea->enable(0);
                    decisionArea->enable(1);
                    tipArea->setMsg(QStringLiteral("你被虚弱了，要强摸")+QString::number(cmd->args(1))+QStringLiteral("张牌么？"));
                }
                break;
            case network::RESPOND_HEAL:
            {
                // 治疗询问
                targetID=cmd->args(0);
                howMany=cmd->args(1);
                int max_avail = cmd->args(3);
                gui->reset();
                if(targetID==myID)
                    myRole->cure(playerList[myID]->getCrossNum(),howMany,cmd->args(2),max_avail);
                break;
            }
            case network::RESPOND_BULLET:
            {
                int nextID;
                targetID=cmd->args(0);
                sourceID=cmd->args(1);
                howMany=cmd->args(2);
                nextID=cmd->args(3);
                char str[10];
                sprintf(str, "%d", howMany);
                gui->logAppend(playerList[sourceID]->getRoleName()+QStringLiteral("对")+playerList[targetID]->getRoleName()+QStringLiteral("使用了魔弹，目前伤害为：")+QString(str)+QStringLiteral("点"));
                if(targetID!=myID)
                {
                    gui->setEnable(0);
                }
                else
                {
                    gui->setEnable(1);
                    myRole->moDaned(nextID,sourceID,howMany);
                }
                break;
            }
            case network::ACTION_ANY:
                targetID = cmd->src_id();
                if(targetID!=myID)
                {
                    isMyTurn=0;
                    gui->setEnable(0);
                    actionFlag=-1;
                }
                else
                {
                    gui->setEnable(1);
                    myRole->normal();
                }
                break;
            case network::ACTION_ATTACK_MAGIC:
                targetID = cmd->src_id();
                if(targetID!=myID)
                {
                    isMyTurn=0;
                    gui->setEnable(0);
                    actionFlag=-1;
                }
                else
                {
                    gui->setEnable(1);
                    myRole->attackOrMagic();
                }
                break;
            case network::ACTION_ATTACK:
                targetID = cmd->src_id();
                if(targetID!=myID)
                {
                    isMyTurn=0;
                    gui->setEnable(0);
                    actionFlag=-1;
                }
                else
                {
                    gui->setEnable(1);
                    myRole->attackAction();
                }
                break;
            case network::ACTION_MAGIC:
                targetID = cmd->src_id();
                if(targetID!=myID)
                {
                    isMyTurn=0;
                    gui->setEnable(0);
                    actionFlag=-1;
                }
                else
                {
                    gui->setEnable(1);
                    myRole->magicAction();
                }
                break;
            case network::ACTION_NONE:
                targetID = cmd->src_id();
                if(targetID!=myID)
                {
                    isMyTurn=0;
                    gui->setEnable(0);
                    actionFlag=-1;
                }
                else
                {
                    gui->setEnable(1);
                    decisionArea->enable(3);
                }
                break;
            case network::RESPOND_ADDITIONAL_ACTION:
                targetID = cmd->src_id();
                skillCmd.CopyFrom(*cmd);
                msg=QStringLiteral("等待")+playerList[targetID]->getRoleName()+QStringLiteral("额外行动响应");
                gui->logAppend(msg);
                if(targetID == myID){
                    gui->setEnable(1);
                    additionalAction(cmd);
                }
                break;

            default:
                // 其他技能
                targetID = cmd->src_id();
                msg = QStringLiteral("等待") + playerList[targetID]->getRoleName() + getCauseString(cmd->respond_id()) + QStringLiteral("响应");
                gui->logAppend(msg);

                if(targetID==myID)
                {
                    gui->setEnable(1);
                    gui->alert();

                    skillCmd.CopyFrom(*cmd);
                    myRole->askForSkill(cmd);
                }
                else{
                    gui->setEnable(0);
                }
                break;
            }
            if(targetID == myID){
                timelineBar->startCounting(59);
            }
        }
        break;
    }

    case network::MSG_CARD:
    {
        network::CardMsg* cardMsg = (network::CardMsg*)proto;
        if(cardMsg->type() == CM_USE){
            cardID = cardMsg->card_ids(0);
            targetID = cardMsg->dst_id();
            sourceID = cardMsg->src_id();
            card = dataInterface->getCard(cardID);
            if(targetID!=network::PLAYER_NONE_ID && targetID!=sourceID){
                playerArea->drawLineBetween(sourceID,targetID);
            }
            if(cardMsg->is_real()){
                cards.clear();
                cards<<card;
                showArea->showCards(cards);
            }
            gui->logAppend(playerList[sourceID]->getRoleName()+QStringLiteral("对")+playerList[targetID]->getRoleName()+QStringLiteral("使用了<font color=\'orange\'>")+card->getInfo()+"</font>");
        }
        else if(cardMsg->type() == CM_SHOW){
            sourceID = cardMsg->src_id();
            cards.clear();
            msg = playerList[sourceID]->getRoleName()+QStringLiteral("展示了：<font color=\'orange\'>");
            for(int i = 0; i < cardMsg->card_ids_size(); i++){
                cardID = cardMsg->card_ids(i);
                card = dataInterface->getCard(cardID);
                msg += card->getInfo() + " ";
                cards<<card;
            }
            showArea->showCards(cards);
            gui->logAppend(msg+"</font>");
        }
        break;
    }
    case network::MSG_ACTION:
    {
        network::Action* action = (network::Action*)proto;

        switch (action->action_type())
        {
        case network::ACTION_ATTACK:
            break;
        case network::ACTION_SPECIAL:
            targetID = action->src_id();
            msg = playerList[targetID]->getRoleName();
            if(action->action_id() == SPECIAL_BUY)
                msg += QStringLiteral("执行购买");
            else if(action->action_id() == SPECIAL_SYNTHESIZE)
                msg += QStringLiteral("执行合成");
            else if(action->action_id() == SPECIAL_EXTRACT)
                msg += QStringLiteral("执行提炼");
            gui->logAppend(msg);
            break;
        case network::ACTION_UNACTIONAL:
            targetID=action->src_id();
            msg=playerList[targetID]->getRoleName()+QStringLiteral("宣告无法行动");
            gui->logAppend(msg);
            break;
        default:
            break;
        }

        break;
    }
    case network::MSG_RESPOND:
    {
        network::Respond* respond = (network::Respond*)proto;

        switch (respond->respond_id())
        {
        case network::RESPOND_DISCARD:
            break;
        case network::RESPOND_BULLET:
            int nextID;
            targetID=respond->dst_ids(0);
            sourceID=respond->src_id();
            howMany=respond->args(0);
            nextID=respond->args(1);
            gui->logAppend(playerList[sourceID]->getRoleName()+QStringLiteral("对")+playerList[targetID]->getRoleName()+QStringLiteral("使用了魔弹，目前伤害为：")+QString::number(howMany)+QStringLiteral("点"));
            break;
        case network::RESPOND_REPLY_ATTACK:
            break;
        default:
            break;
        }

        break;
    }

    case network::MSG_HIT:
    // 命中通告
    {
        network::HitMsg* hit_msg = (network::HitMsg*)proto;
        targetID=hit_msg->dst_id();
        sourceID=hit_msg->src_id();
        if(hit_msg->hit())
            msg=playerList[sourceID]->getRoleName()+QStringLiteral("命中了")+playerList[targetID]->getRoleName();
        else
            msg=playerList[sourceID]->getRoleName()+QStringLiteral("未命中")+playerList[targetID]->getRoleName();
        gui->logAppend(msg);
        QSound("sound/Hit.wav");
        break;
    }

    case network::MSG_HURT:
    {
        network::HurtMsg* hurt_msg = (network::HurtMsg*)proto;
        sourceID = hurt_msg->src_id();
        targetID = hurt_msg->dst_id();
        if(hurt_msg->type() == HARM_ATTACK){
            msg=playerList[sourceID]->getRoleName()+QStringLiteral("对")+playerList[targetID]->getRoleName()+QStringLiteral("造成了")+QString::number(hurt_msg->hurt())+QStringLiteral("点攻击伤害");
        }
        else{
            msg=playerList[sourceID]->getRoleName()+QStringLiteral("的")+getCauseString(hurt_msg->cause())+QStringLiteral("对")+playerList[targetID]->getRoleName()+QStringLiteral("造成了")+QString::number(hurt_msg->hurt())+QStringLiteral("点法术伤害");
        }
        gui->logAppend(msg);
        playerArea->drawLineBetween(sourceID, targetID);
        QSound("sound/Hurt.wav");
        break;
    }
    case network::MSG_SKILL:
    {
        network::SkillMsg* skill = (network::SkillMsg*)proto;
        sourceID = skill->src_id();
        if(skill->dst_ids(0)!=-1){
            msg = playerList[sourceID]->getRoleName()+QStringLiteral("对");
            for(i = 0; i < skill->dst_ids_size()-1; i++){
                targetID = skill->dst_ids(i);
                msg += playerList[targetID]->getRoleName()+QStringLiteral("、");
            }
            msg += playerList[skill->dst_ids(i)]->getRoleName() + QStringLiteral("发动") + getCauseString(skill->skill_id());
        }
        else{
            msg = playerList[sourceID]->getRoleName() + QStringLiteral("发动") + getCauseString(skill->skill_id());
        }
        gui->logAppend(msg);
        break;
    }
    case network::MSG_GOSSIP:
    // 对话及公告
    {
        network::Gossip* gossip = (network::Gossip*)proto;
        switch (gossip->type())
        {
        case network::GOSSIP_TALK:
            gui->chatAppend(gossip->id(), QString(gossip->txt().c_str()));
            break;
        case network::GOSSIP_NOTICE:
            gui->logAppend(QString(gossip->txt().c_str()));
            break;
        }
        break;
    }
    case network::MSG_GAME:
    // board更新
    {
        network::GameInfo* game_info = (network::GameInfo*)proto;
        qDebug( "Role Recieve: \n%s", proto->DebugString().c_str());

        int winner = -1;

        // 更新士气
        if (game_info->has_red_morale())
        {
            if (red->getMorale() > game_info->red_morale())
                QSound("sound/Hurt.wav");
            QSound("sound/Morale.wav");
            red->setMorale(game_info->red_morale());
            teamArea->update();
            if (red->getMorale() == 0)
                winner = 0;
        }
        if (game_info->has_blue_morale())
        {
            if (blue->getMorale() > game_info->blue_morale())
                QSound("sound/Hurt.wav");
            QSound("sound/Morale.wav");
            blue->setMorale(game_info->blue_morale());
            teamArea->update();
            if (blue->getMorale() == 0)
                winner = 1;
        }
        // 更新星杯
        if (game_info->has_red_grail())
        {
            red->setGrail(game_info->red_grail());
            teamArea->update();
            if (red->getGrail() == 5)
                winner = 1;
        }
        if (game_info->has_blue_grail())
        {
            blue->setGrail(game_info->blue_grail());
            teamArea->update();
            if (blue->getGrail() == 5)
                winner = 0;
        }

        // 胜负已分
        if (winner == 0 || winner == 1)
        {
            tipArea->win(winner);
            if(winner==dataInterface->getMyself()->getColor())
                QSound("sound/Win.wav");
            else
                QSound("sound/Lose.wav");
        }

        // 更新战绩区
        if (game_info->has_red_gem())
        {
            red->setGem(game_info->red_gem());
            QSound("sound/Stone.wav");
        }
        if (game_info->has_blue_gem())
        {
            blue->setGem(game_info->blue_gem());
            QSound("sound/Stone.wav");
        }
        if (game_info->has_red_crystal())
        {
            red->setCrystal(game_info->red_crystal());
            QSound("sound/Stone.wav");
        }
        if (game_info->has_blue_crystal())
        {
            blue->setCrystal(game_info->blue_crystal());
            QSound("sound/Stone.wav");
        }
        // 更新牌堆、弃牌堆
        if (game_info->has_pile())
        {
            teamArea->setLeftCardNum(game_info->pile());
        }
        if (game_info->has_discard())
        {
            if (game_info->discard()==0 && teamArea->getDroppedCardNum() != 0)
            {
                gui->logAppend(QStringLiteral("牌堆重洗"));
                QSound("sound/Shuffle.wav");
            }
            teamArea->setDroppedCardNum(game_info->discard());
        }
        // 更新玩家信息
        if (game_info->player_infos_size() > 0)
        {
            network::SinglePlayerInfo* player_info;
            for (int i = 0; i < game_info->player_infos_size(); ++i)
            {
                player_info = (network::SinglePlayerInfo*)&(game_info->player_infos(i));
                targetID = player_info->id();

                player = playerList[targetID];

                // 更新手牌
                if (player_info->has_hand_count())
                {
                    int offset = player_info->hand_count() - player->getHandCardNum();
                    if (offset > 0)
                        gui->logAppend(player->getRoleName()+QStringLiteral("的手牌增加了")+QString::number(offset)+QStringLiteral("张"));
                    else if (offset < 0)
                        gui->logAppend(player->getRoleName()+QStringLiteral("的手牌减少了")+QString::number(-offset)+QStringLiteral("张"));

                    if(player_info->hand_count() != player->getHandCardNum())
                        player->changeHandCardNumTo(player_info->hand_count());

                    if (targetID == myID)
                    {
                        dataInterface->cleanHandCard();
                        for (int k = 0; k < player_info->hands_size(); ++k)
                        {
                            cardID=player_info->hands(k);
                            card=dataInterface->getCard(cardID);
                            dataInterface->addHandCard(card);
                        }
                    }
                }
                // 更新盖牌
                if (player_info->has_covered_count())
                {
                    char str[10];
                    sprintf(str, "%d", player_info->covered_count());
                    QString q_str(str);
                    gui->logAppend(player->getRoleName()+QStringLiteral("的盖牌变为")+q_str+QStringLiteral("张"));
                    player->setToken(2, player_info->covered_count());

                    if (targetID == myID)
                    {
                        dataInterface->cleanCoverCard();
                        for (int k = 0; k < player_info->covered_count(); ++k){
                            cardID=player_info->covereds(k);
                            card=dataInterface->getCard(cardID);
                            dataInterface->addCoverCard(card);
                        }
                    }
                }
                // 更新治疗
                if (player_info->has_heal_count())
                {
                    if (player->getCrossNum() > player_info->heal_count())
                    {
                       howMany = player->getCrossNum() - player_info->heal_count();
                       msg=playerList[targetID]->getRoleName()+QStringLiteral("减少了")+QString::number(howMany)+QStringLiteral("点治疗");
                       gui->logAppend(msg);
                    }
                    else
                    {
                        howMany = player_info->heal_count() - player->getCrossNum();
                        msg=playerList[targetID]->getRoleName()+QStringLiteral("增加了")+QString::number(howMany)+QStringLiteral("点治疗");
                        gui->logAppend(msg);
                    }
                    player->setCrossNum(player_info->heal_count());

                    playerArea->update();
                    QSound("sound/Cure.wav");
                }
                // 更新专属
                if (player_info->ex_cards_size() > 0)
                {
                    player->cleanSpecial();
                    for (int j = 0; j < player_info->ex_cards_size(); ++j)
                    {
                        player->setSpecial(player_info->ex_cards(j), 1);
                    }
                }
                // 更新基础效果
                if (player_info->basic_cards_size() > 0)
                {
                    if (player_info->basic_cards_size() > player->getBasicStatus().size())
                        QSound("sound/Equip.wav");

                    player->cleanBasicStatus();
                    for (int j = 0; j < player_info->basic_cards_size(); ++j)
                    {
                        cardID = player_info->basic_cards(j);
                        card = dataInterface->getCard(cardID);
                        cardName = card->getName();
                        if(cardName == QStringLiteral("中毒"))
                            player->addBasicStatus(0, card);
                        else if(cardName == QStringLiteral("虚弱"))
                            player->addBasicStatus(1, card);
                        else if(cardName == QStringLiteral("圣盾") || card->getSpecialityList().contains(QStringLiteral("天使之墙")))
                            player->addBasicStatus(2, card);
                        else if(card->getType() == "attack" && card->getProperty()==QStringLiteral("幻"))
                            player->addBasicStatus(3, card);
                        else if(card->getSpecialityList().contains(QStringLiteral("威力赐福")))
                            player->addBasicStatus(4, card);
                        else if(card->getSpecialityList().contains(QStringLiteral("迅捷赐福")))
                            player->addBasicStatus(5, card);
                    }
                }
                // 更新能量
                if (player_info->has_gem())
                {
                    player->setGem(player_info->gem());
                }
                if (player_info->has_crystal())
                {
                    player->setCrystal(player_info->crystal());
                }
                // 更新标记物
                if (player_info->has_yellow_token())
                {
                    player->setToken(0, player_info->yellow_token());
                }
                if (player_info->has_blue_token())
                {
                    player->setToken(1, player_info->blue_token());
                }
                // 更新玩家横置状态
                if (player_info->has_is_knelt())
                {
                    // 横置
                    if(!player_info->is_knelt())
                    {
                        playerList[targetID]->setTap(0);
                        msg=playerList[targetID]->getRoleName()+QStringLiteral("重置");
                    }
                    else
                    {
                        playerList[targetID]->setTap(1);
                        msg=playerList[targetID]->getRoleName()+QStringLiteral("横置");
                    }
                    playerArea->update();
                    gui->logAppend(msg);
                }
                // 最大手牌改变
                if (player_info->has_max_hand())
                {
                    howMany=player_info->max_hand();
                    player->setHandCardsMax(howMany);
                }
                // 清空数组
                if (player_info->delete_field_size() > 0)
                {
                    for (int j = 0; j < player_info->delete_field_size(); ++j)
                    {
                        if (strcmp(player_info->delete_field(j).c_str(), "ex_cards") == 0)
                        {
                            player->cleanSpecial();
                        }
                        else if (strcmp(player_info->delete_field(j).c_str(), "basic_cards") == 0)
                        {
                            player->cleanBasicStatus();
                        }
                    }
                }
                playerArea->update();
            }
        }
        break;
    }
    case network::MSG_ERROR:
    {
        network::Error* error = (network::Error*)proto;
        if(error->dst_id() == myID){
            gui->reset();
        }
        switch(error->id())
        {
        case GE_DISCONNECTED:
            gui->logAppend(QStringLiteral("<font color=\'red\'>玩家") + QString::number(error->dst_id()) + QStringLiteral("掉线，请等他回来，或者票死他</font>"));
            break;
        default:
            gui->logAppend(QStringLiteral("<font color=\'red\'>错误代码") + QString::number(error->id()) + "</font>");
            break;
        }
        break;
    }
    case network::MSG_POLLING_REQ:
    {
        state = POLLING;
        network::PollingRequest* req = (network::PollingRequest*) proto;
        gui->hideBP();
        gui->reset();
        tipArea = gui->getTipArea();
        tipArea->setMsg(QString::fromStdString(req->object()));
        for(int i = 0; i < req->options_size(); i++){
            tipArea->addBoxItem(QString::fromStdString(req->options(i)));
        }
        tipArea->showBox();

        decisionArea = gui->getDecisionArea();
        decisionArea->enable(0);
        timelineBar->startCounting(59);
        gui->alert();
        break;
    }
    default:
        break;
    }
    delete proto;
    }catch(int error){
        logic->onError(error);
    }
}


