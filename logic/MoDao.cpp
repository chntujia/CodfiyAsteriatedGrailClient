#include "MoDao.h"

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
    myID=myself->getID();

    int myColor=myself->getColor();

    network::GameInfo* gameInfo=dataInterface->getGameInfo();
    int playerMax=dataInterface->getPlayerMax();
    QList<Player*>playerList=dataInterface->getPlayerList();
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
    if(firstMoDan&&isMyTurn)
        state=801;
    else
        state=802;
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
    state=803;
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
    state=804;
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

    QList<Card*> selectedCards=handArea->getSelectedCards();
    QString cardName;

    switch (state)
    {
//normal action
    case 1:
        cardName=selectedCards[0]->getName();
        if(cardName==QStringLiteral("魔弹"))
            playerArea->enablePlayerItem(nextClockwise);//额外再激活一个上家敌人
        break;
//魔弹融合(首次魔弹）
    case 801:
        playerArea->enablePlayerItem(nextClockwise);
        playerArea->enablePlayerItem(nextCounterClockwise);
        break;
//魔弹融合(非首次）
    case 802:
        playerArea->enablePlayerItem(moDanNextID);
        break;
//魔爆冲击
    case 803:
        playerArea->enableEnemy();
        break;
    }
}

void MoDao::onOkClicked()
{    
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    QString command;
    QString cardID;
    QString sourceID;
    QString targetID;
    QString targetID2;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

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
        if(!selectedCards.empty() && selectedCards[0]->getName()==QStringLiteral("魔弹"))
            firstMoDan=false;
        break;
//魔弹融合(回合内）
    case 801:
        action = newAction(801);
        action->add_args(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());

        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        firstMoDan=false;
        break;
//魔弹融合(回合外）
    case 802:
        respond = newRespond(802);
        respond->add_args(selectedCards[0]->getID());
        respond->add_dst_ids(selectedPlayers[0]->getID());

        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//魔爆冲击
    case 803:
        action = newAction(803);
        action->add_args(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());
        action->add_dst_ids(selectedPlayers[1]->getID());

        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//毁灭风暴
    case 804:
        action = newAction(803);
        action->add_args(1);
        action->add_dst_ids(selectedPlayers[0]->getID());
        action->add_dst_ids(selectedPlayers[1]->getID());

        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    }
    Role::onOkClicked();
}

void MoDao::onCancelClicked()
{
    Role::onCancelClicked();
    switch(state)
    {
//魔弹融合(回合内）
    case 801:
//魔爆冲击
    case 803:
//毁灭风暴
    case 804:
        normal();
        break;
//魔弹融合(回合外）
    case 802:
        moDaned(moDanNextID,sourceID,moDanHarm);
        break;
    }
}

