#include "MoDao.h"

enum CAUSE{
    MO_BAO_CHONG_JI = 801,
    MO_DAN_ZHANG_WO = 802,
    MO_DAN_RONG_HE = 803,
    HUI_MIE_FENG_BAO = 804
};

enum REATTACK{
    RA_ATTACK,
    RA_BLOCK,
    RA_GIVEUP
};
MoDao::MoDao()
{
    makeConnection();
    setMyRole(this);

    Button *moDanRongHe,*moBaoChongJi,*huiMeiFengBao;
    moDanRongHe=new Button(3,QStringLiteral("魔弹融合"));
    buttonArea->addButton(moDanRongHe);
    connect(moDanRongHe,SIGNAL(buttonSelected(int)),this,SLOT(MoDanRongHe()));

    moBaoChongJi=new Button(4,QStringLiteral("魔爆冲击"));
    buttonArea->addButton(moBaoChongJi);
    connect(moBaoChongJi,SIGNAL(buttonSelected(int)),this,SLOT(MoBaoChongJi()));

    huiMeiFengBao=new Button(5,QStringLiteral("毁灭风暴"));
    buttonArea->addButton(huiMeiFengBao);
    connect(huiMeiFengBao,SIGNAL(buttonSelected(int)),this,SLOT(HuiMeiFengBao()));

    Player*myself=dataInterface->getMyself();

    int myColor=myself->getColor();

    network::GameInfo* gameInfo=dataInterface->getGameInfo();
    int playerMax=dataInterface->getPlayerMax();
    SafeList<Player*>playerList=dataInterface->getPlayerList();
    int i;
    //find myPos
    for(i=0;i<playerMax;i++)
        if(gameInfo->player_infos(i).id()==myID)
            break;
    int ptr;
    do
    {
        i--;
        if(i==-1)
            i=playerMax-1;
        ptr=gameInfo->player_infos(i).id();
    }while(playerList[ptr]->getColor()==myColor);
    nextClockwise=ptr;
}

void MoDao::MoDanRongHe()
{
    state=MO_DAN_RONG_HE;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("earth");
    handArea->enableElement("fire");
}

void MoDao::MoBaoChongJi()
{
    state=MO_BAO_CHONG_JI;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(2);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableMagic();
}

void MoDao::HuiMeiFengBao()
{
    state=HUI_MIE_FENG_BAO;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(2);

    decisionArea->enable(1);
    decisionArea->disable(0);

    playerArea->enableEnemy();
}

void MoDao::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
//魔弹融合
    if (handArea->checkElement("earth")||handArea->checkElement("fire"))
        buttonArea->enable(3);
//魔爆冲击
    if (handArea->checkType("magic"))
        buttonArea->enable(4);
//毁灭风暴
    if(myself->getGem()>0)
        buttonArea->enable(5);
    unactionalCheck();
}
void MoDao::turnBegin()
{
    Role::turnBegin();
    firstMoDan=true;
}

void MoDao::moDaned(int nextID,int sourceID, int howMany)
{
    moDanHarm=howMany;
    Role::moDaned(nextID,sourceID,howMany);
    firstMoDan=false;

    if (handArea->checkElement("earth")||handArea->checkElement("fire"))
        buttonArea->enable(3);
}

void MoDao::cardAnalyse()
{
    Role::cardAnalyse();

    SafeList<Card*> selectedCards=handArea->getSelectedCards();
    QString cardName;
    try{
    switch (state)
    {
//normal action
    case 1:
        cardName=selectedCards[0]->getName();
        if(cardName==QStringLiteral("魔弹"))
            playerArea->enablePlayerItem(nextClockwise);//额外再激活一个上家敌人
        break;
//魔弹融合
    case MO_DAN_RONG_HE:
        //发起魔弹
        if(isMyTurn && firstMoDan){
            playerArea->enablePlayerItem(nextClockwise);
            playerArea->enablePlayerItem(nextCounterClockwise);
        }
        //响应魔弹
        else{
            playerArea->enablePlayerItem(moDanNextID);
        }
        break;
//魔爆冲击
    case MO_BAO_CHONG_JI:
        playerArea->enableEnemy();
        break;
    }
    }catch(int error){
        logic->onError(error);
    }
}

void MoDao::onOkClicked()
{    
    SafeList<Card*> selectedCards;
    SafeList<Player*>selectedPlayers;

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
        if(!selectedCards.empty() && selectedCards[0]->getName()==QStringLiteral("魔弹"))
            firstMoDan=false;
        break;
//魔弹融合
    case MO_DAN_RONG_HE:
        // 发起魔弹
        if(isMyTurn && firstMoDan){
            firstMoDan=false;
            action = newAction(ACTION_MAGIC_SKILL, MO_DAN_RONG_HE);
            action->add_card_ids(selectedCards[0]->getID());
            action->add_dst_ids(selectedPlayers[0]->getID());
            emit sendCommand(network::MSG_ACTION, action);
        }
        // 响应魔弹
        else{
            respond = newRespond(RESPOND_BULLET);
            respond->add_args(RA_ATTACK);
            respond->add_card_ids(selectedCards[0]->getID());
            respond->add_dst_ids(selectedPlayers[0]->getID());
            emit sendCommand(network::MSG_RESPOND, respond);
        }
        gui->reset();

        break;
//魔爆冲击
    case MO_BAO_CHONG_JI:
        action = newAction(ACTION_MAGIC_SKILL, MO_BAO_CHONG_JI);
        action->add_card_ids(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());
        action->add_dst_ids(selectedPlayers[1]->getID());

        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//毁灭风暴
    case HUI_MIE_FENG_BAO:
        action = newAction(ACTION_MAGIC_SKILL, HUI_MIE_FENG_BAO);
        action->add_dst_ids(selectedPlayers[0]->getID());
        action->add_dst_ids(selectedPlayers[1]->getID());

        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    }
    }catch(int error){
        logic->onError(error);
    }
    Role::onOkClicked();
}

void MoDao::onCancelClicked()
{
    Role::onCancelClicked();
    switch(state)
    {
//魔弹融合
    case MO_DAN_RONG_HE:
        // 发起魔弹
        if(isMyTurn && firstMoDan){
            normal();
        }
        else{
            moDaned(moDanNextID,sourceID,moDanHarm);
        }
        break;
//魔爆冲击
    case MO_BAO_CHONG_JI:
//毁灭风暴
    case HUI_MIE_FENG_BAO:
        normal();
        break;
    }
}

