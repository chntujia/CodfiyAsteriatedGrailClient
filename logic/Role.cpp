#include "Role.h"
#include <QStringList>
#include <QSound>
#include "data/DataInterface.h"
#include "widget/GUI.h"
#include "logic/Logic.h"

Role::Role(QObject *parent) :
    QObject(parent)
{
    Player*myself=dataInterface->getMyself();
    myID=myself->getID();
    int myColor=myself->getColor();

    QString queue=dataInterface->getQueue();
    int playerMax=queue.count()/2;
    red=dataInterface->getRedTeam();
    blue=dataInterface->getBlueTeam();
    playerList=dataInterface->getPlayerList();
    int i;
    //find myPos
    for(i=0;i<playerMax;i++)
        if(queue[i].digitValue()==myID)
            break;
    int ptr;
    do
    {
        i++;
        if(i==playerMax)
            i=0;
        ptr=queue[i].digitValue();        

    }while(playerList[ptr]->getColor()==myColor);
    nextCounterClockwise=ptr;
    handArea=gui->getHandArea();
    playerArea=gui->getPlayerArea();
    buttonArea=gui->getButtonArea();
    decisionArea=gui->getDecisionArea();
    tipArea=gui->getTipArea();
    teamArea=gui->getTeamArea();
    coverArea = gui->getCoverArea();    
    showArea=gui->getShowArea();

}
void Role::makeConnection()
{
    connect(logic->getClient(),SIGNAL(getMessage(QString)),this,SLOT(decipher(QString)));
    connect(this,SIGNAL(sendCommand(QString)),logic->getClient(),SLOT(sendMessage(QString)));
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
    QList<Card*> selectedCards=handArea->getSelectedCards();
    QString cardName;
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
    QString command="30;";
    QList<Card*> handcards=dataInterface->getHandCards();
    int i;
    int n=handcards.count();
    for(i=0;i<n;i++)
        dataInterface->removeHandCard(handcards[i]);
    emit sendCommand(command);
}

void Role::resign()
{
    emit sendCommand("4;-1");
    gui->reset();
}

//正常行动
void Role::normal()
{
    gui->reset();

    QList<Card*> handcards=dataInterface->getHandCards();
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
    QList<Button*>buttons=buttonArea->getButtons();
    QList<CardItem*>cardItems=handArea->getHandCardItems();
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
    QSound::play("sound/Warning.wav");
}

void Role::drop(int howMany)
{
    state=3;
    handArea->setQuota(howMany);
    handArea->enableAll();
    gui->alert();

}

void Role::dropCover(int howMany)
{
    state = 50;
    gui->showCoverArea(true);
    HandArea *coverArea = gui->getCoverArea();
    coverArea->reset();
    coverArea->setQuota(howMany);
    coverArea->enableAll();
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
    QSound::play("sound/Warning.wav");
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
    QSound::play("sound/Warning.wav");
}

void Role::turnBegin()
{
    isMyTurn=true;
    onceUsed=false;
    start=false;
    usedAttack=usedMagic=usedSpecial=false;
    gui->alert();
    QSound::play("sound/Warning.wav");
}

void Role::additionalAction(){
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否执行额外行动？"));
    if(dataInterface->getMyself()->checkBasicStatus(5))
        gui->getTipArea()->addBoxItem("0.迅捷赐福");
    state=42;
    tipArea->showBox();
    decisionArea->enable(0);
    decisionArea->enable(3);
    gui->alert();
    QSound::play("sound/Warning.wav");
}

void Role::askForSkill(QString skill)
{
    gui->reset();
    if(skill==QStringLiteral("威力赐福"))
        WeiLi();
    gui->alert();
    QSound::play("sound/Warning.wav");
}

void Role::TianShiZhuFu(int n)
{
    gui->reset();
    state=751;
    tipArea->setMsg(QStringLiteral("给予天使")+QString::number(n)+QStringLiteral("张牌"));
    handArea->setQuota(n);
    handArea->enableAll();
    gui->alert();
    QSound::play("sound/Warning.wav");
}

void Role::MoBaoChongJi()
{
    gui->reset();
    state=851;
    tipArea->setMsg(QStringLiteral("弃一张法术牌或受到两点法术伤害"));
    handArea->setQuota(1);
    handArea->enableMagic();
    decisionArea->enable(1);
    gui->alert();
    QSound::play("sound/Warning.wav");
}

void Role::WeiLi()
{
    state=36;
    tipArea->setMsg(QStringLiteral("是否发动威力赐福？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void Role::ChongYing(int color)
{
    gui->reset();
    state=2951;
    Player* myself=dataInterface->getMyself();
    if(myself->getRoleName()!=QStringLiteral("[魔枪]"))
        tipArea->setMsg(QStringLiteral("弃一张牌，法术或雷将会为魔枪加1伤害"));
    else
        tipArea->setMsg(QStringLiteral("可弃一张牌"));
    if(handArea->getHandCardItems().size()==0)
        decisionArea->enable(3);
    else if(color==myself->getColor())
    {
        decisionArea->enable(1);
        handArea->enableAll();
        handArea->setQuota(1);
    }
    else
    {
        handArea->enableAll();
        handArea->setQuota(1);
    }
    decisionArea->disable(0);
    gui->alert();
}

void Role::onCancelClicked()
{
    QString command;
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
        command="6;2;;;"+QString::number(myID)+";";
        emit sendCommand(command);
        gui->reset();
        break;
//虚弱
    case 7:
        command="23;1;";
        emit sendCommand(command);
        gui->reset();
        break;
//魔弹应答
    case 8:
        command="27;2;;;";
        emit sendCommand(command);
        gui->reset();
        break;
//治疗应答
    case 9:
        command="34;0;";
        emit sendCommand(command);
        gui->reset();
        break;
//简单的技能发动询问
    case 36:
        command="36;0;";
        emit sendCommand(command);
        gui->reset();
        break;
//魔爆冲击弃牌
    case 851:
        command="851;0;";
        emit sendCommand(command);
        gui->reset();
        break;
//充盈弃牌
    case 2951:
        command="2951;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void Role::onOkClicked()
{
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    QString command;
    QString cardID;
    QString sourceID;
    QString targetID;
    QString text;
    int i,boxCurrentIndex;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    switch(state)
    {
//NORMALACTION
    case 1:
//追加行动
    case 10:
    case 11:
    case 12:
        cardID=QString::number(selectedCards[0]->getID());
        targetID=QString::number(selectedPlayers[0]->getID());
        sourceID=QString::number(myID);
        if(selectedCards[0]->getType()=="attack"){
            command="4;1;"+cardID+";"+targetID+";"+sourceID+";";
            usedAttack=true;
            usedMagic=usedSpecial=false;
        }
        else
        {
            QString cardName;
            cardName=selectedCards[0]->getName();
            if(cardName==QStringLiteral("中毒")||cardName==QStringLiteral("虚弱")||cardName==QStringLiteral("圣盾")||cardName==QStringLiteral("魔弹"))
                command="4;2;0;"+cardID+";"+targetID+";"+sourceID+";";
            usedMagic=true;
            usedAttack=usedSpecial=false;
        }
        dataInterface->removeHandCard(selectedCards[0]);
        gui->reset();
        emit sendCommand(command);
        break;
//ATTACKEDREPLY
    case 2:
        command="6;";
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        if(selectedCards[0]->getType()=="attack")
        {
            targetID=QString::number(selectedPlayers[0]->getID());
            command+="0;"+cardID+";"+targetID+";"+sourceID+";";
        }
        else if(selectedCards[0]->getElement()=="light")
            command+="1;"+cardID+";;"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        gui->reset();
        emit sendCommand(command);
        break;
//DROPREPLY
    case 3:
        cardID=QString::number(selectedCards[0]->getID());
        command="8;"+cardID;
        for(i=1;i<selectedCards.count();i++)
        {
            cardID=QString::number(selectedCards[i]->getID());
            command+=","+cardID;
        }
        command+=";";
        for(i=0;i<selectedCards.count();i++)
        {
            dataInterface->removeHandCard(selectedCards[i]);
        }
        gui->reset();
        emit sendCommand(command);
        break;
//BUY
    case 4:
        command="4;3;0;";
        boxCurrentIndex=tipArea->getBoxCurrentIndex();

        switch(boxCurrentIndex)
        {
        case -1:
            if(dataInterface->getMyTeam()->getEnergy()<4)
                command+="1;1;";
            else
                command+="0;0;";
            break;
        case 0:
            command+="1;0;";
            break;
        case 1:
            command+="0;1;";
            break;
        }
        gui->reset();
        usedSpecial=true;
        usedAttack=usedMagic=false;
        emit sendCommand(command);
        break;
//SYNTHETIZE
    case 5:
        command="4;3;1;";
        text=tipArea->getBoxCurrentText();
        switch(text[0].digitValue())
        {
        case 1:
            command+="0;3;";
            break;
        case 2:
            command+="1;2;";
            break;
        case 3:
            command+="2;1;";
            break;
        case 4:
            command+="3;0;";
            break;
        }
        gui->reset();
        usedSpecial=true;
        usedAttack=usedMagic=false;
        emit sendCommand(command);
        break;
//EXTRACT
    case 6:
        command="4;3;2;";
        text=tipArea->getBoxCurrentText();
        switch(text[0].digitValue())
        {
        case 1:
            command+="2;0;";
            break;
        case 2:
            command+="0;2;";
            break;
        case 3:
            command+="1;1;";
            break;
        case 4:
            command+="1;0;";
            break;
        case 5:
            command+="0;1;";
            break;
        }
        gui->reset();
        usedSpecial=true;
        usedAttack=usedMagic=false;
        emit sendCommand(command);
        break;
//虚弱
    case 7:
        command="23;0;";
        gui->reset();
        emit sendCommand(command);
        break;
//魔弹应答
    case 8:
        if(selectedCards[0]->getName()==QStringLiteral("圣光"))
        {
            cardID=QString::number(selectedCards[0]->getID());
            command="27;1;"+cardID+";;";
        }
        else if(selectedCards[0]->getName()==QStringLiteral("魔弹"))
        {
            cardID=QString::number(selectedCards[0]->getID());
            command="27;0;"+cardID+";";
            targetID=QString::number(selectedPlayers[0]->getID());
            command+=targetID+";";
        }
        dataInterface->removeHandCard(selectedCards[0]);
        gui->reset();
        emit sendCommand(command);
        break;
//治疗应答
    case 9:
        text=tipArea->getBoxCurrentText();
        command="34;"+text+";";
        gui->reset();
        emit sendCommand(command);
        break;
//简单的技能发动询问
    case 36:
        command="36;1;";
        emit sendCommand(command);
        gui->reset();
        break;
//额外行动（迅捷）
    case 42:
        if(tipArea->getBoxCurrentText().at(0).digitValue()==0)
        {
            emit sendCommand("1607;"+QString::number(myID)+";");
            myRole->attackAction();
        }
        break;
//弃盖牌
    case 50:
        selectedCards = coverArea->getSelectedCards();
        cardID=QString::number(selectedCards[0]->getID());
        command="50;"+cardID;
        for(i=1;i<selectedCards.count();i++)
        {
            cardID=QString::number(selectedCards[i]->getID());
            command+=","+cardID;
        }
        command+=";";
        for(i=0;i<selectedCards.count();i++)
        {
            dataInterface->removeHandCard(selectedCards[i]);
        }
        coverArea->reset();
        gui->showCoverArea(false);
        gui->reset();
        emit sendCommand(command);
        break;
//天使祝福
    case 751:
        command="751;"+QString::number(selectedCards.size())+";";
        cardID=QString::number(selectedCards[0]->getID());
        dataInterface->removeHandCard(selectedCards[0]);
        command+=cardID+";";
        if(selectedCards.size()==2)
        {
            cardID=QString::number(selectedCards[1]->getID());
            command+=cardID+";";
            dataInterface->removeHandCard(selectedCards[1]);
        }
        gui->reset();
        emit sendCommand(command);
        break;
//魔爆冲击
    case 851:
        cardID=QString::number(selectedCards[0]->getID());
        command="851;1;"+cardID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        gui->reset();
        emit sendCommand(command);
        break;
//充盈
    case 2951:
        cardID=QString::number(selectedCards[0]->getID());
        command="2951;1;"+cardID+";";
        if(selectedCards[0]->getElement()=="thunder" || selectedCards[0]->getType()=="magic")
            command+="1;";
        else
            command+="0;";
        dataInterface->removeHandCard(selectedCards[0]);
        gui->reset();
        emit sendCommand(command);
        break;
    }
}

void Role::decipher(QString command)
{
    this->command=command;
    QStringList arg=command.split(';');
    QStringList cardIDList;
    int targetID,targetArea;
    int sourceArea;
    int cardID;
    int hitRate;
    int i,howMany;
    int team,gem,crystal;
    int dir,show;

    Card*card;
    Player*player;    
    QList<Card*> cards;
    QString msg;
    QString flag;
    QString cardName;


    switch (arg[0].toInt())
    {
//回合开始
    case 3:
        targetID=arg[1].toInt();
        gui->logAppend("--------------------------------------");
        gui->logAppend(playerList[targetID]->getRoleName()+QStringLiteral("回合开始"));
        playerArea->setCurrentPlayerID(targetID);

        if(targetID==dataInterface->getFirstPlayerID())
            teamArea->addRoundBy1();
        if(targetID!=myID)
        {
            isMyTurn=0;
            gui->setEnable(0);
        }
        else
        {
            myRole->turnBegin();
            QSound::play("sound/It's your turn.wav");
        }

        break;
//应战询问
    case 5:
        hitRate=arg[1].toInt();
        cardID=arg[2].toInt();
        targetID=arg[3].toInt();
        sourceID=arg[4].toInt();
        card=dataInterface->getCard(cardID);
        QSound::play("sound/Attack.wav");

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
//弃牌询问
    case 7:
        targetID=arg[1].toInt();
        howMany=arg[2].toInt();
        flag=arg[3];
        msg=playerList[targetID]->getRoleName()+QStringLiteral("需要弃")+arg[2]+QStringLiteral("张手牌");
        if(flag=="y")
            gui->logAppend(msg+QStringLiteral("(明弃)"));
        else
            gui->logAppend(msg+QStringLiteral("(暗弃)"));
        if(targetID!=myID)
        {
            gui->setEnable(0);
        }
        else
        {
            gui->setEnable(1);
            gui->reset();
            drop(howMany);
            tipArea->setMsg(QStringLiteral("你需要弃")+arg[2]+QStringLiteral("张手牌"));
        }
        break;
//摸牌
    case 9:
        targetID=arg[1].toInt();
        howMany=arg[2].toInt();
        gui->logAppend(playerList[targetID]->getRoleName()+QStringLiteral("摸取")+arg[2]+QStringLiteral("张手牌"));

        if(targetID==myID)
        {
            cardIDList=arg[3].split(',');
            for(i=0;i<howMany;i++)
            {
                cardID=cardIDList[i].toInt();
                card=dataInterface->getCard(cardID);
                dataInterface->addHandCard(card);
            }
        }
        break;
//牌堆重洗
    case 10:
        gui->logAppend(QStringLiteral("牌堆已重洗"));
        teamArea->setLeftCardNum(arg[1].toInt());
        teamArea->setDroppedCardNum(0);
        QSound::play("sound/Shuffle.wav");
        break;
//士气改变
    case 11:
        team=arg[1].toInt();
        howMany=arg[2].toInt();
        if(team)
        {
            if(red->getMorale()>howMany)
                QSound::play("sound/Hurt.wav");
            red->setMorale(howMany);
            teamArea->update();
        }
        else
        {
            if(blue->getMorale()>howMany)
                QSound::play("sound/Hurt.wav");
            blue->setMorale(howMany);
            teamArea->update();
        }
        QSound::play("sound/Morale.wav");
        break;
//游戏结束
    case 12:
        team=arg[1].toInt();    
        tipArea->win(team);
        if(team==dataInterface->getMyself()->getColor())
            QSound::play("sound/Win.wav");
        else
            QSound::play("sound/Lose.wav");
        break;
//弃牌公告
    case 13:
        targetID=arg[1].toInt();
        howMany=arg[2].toInt();
        flag=arg[3];
        msg=playerList[targetID]->getRoleName()+QStringLiteral("丢弃了")+arg[2]+QStringLiteral("张手牌");
        if(flag=="y")
        {
            cardIDList=arg[4].split(',');
            msg+=":<font color=\'orange\'>";
            cards.clear();
            for(i=0;i<howMany;i++)
            {
                cardID=cardIDList[i].toInt();
                card=dataInterface->getCard(cardID);
                msg+=card->getInfo()+" ";
                cards<<card;
            }
            msg+="</font>";
            showArea->showCards(cards);

        }
        gui->logAppend(msg);
        break;
//命中公告
    case 14:
        flag=arg[1];
        targetID=arg[3].toInt();
        sourceID=arg[4].toInt();        
        if(flag=="1")
            msg=playerList[sourceID]->getRoleName()+QStringLiteral("未命中")+playerList[targetID]->getRoleName();
        else
            msg=playerList[sourceID]->getRoleName()+QStringLiteral("命中")+playerList[targetID]->getRoleName();
        gui->logAppend(msg);
        QSound::play("sound/Hit.wav");
        break;
//星石改变
    case 15:
        team=arg[1].toInt();
        gem=arg[2].toInt();
        crystal=arg[3].toInt();
        if(team)
        {
            red->setGem(gem);
            red->setCrystal(crystal);
            teamArea->update();
        }
        else
        {
            blue->setGem(gem);
            blue->setCrystal(crystal);
            teamArea->update();
        }
        QSound::play("sound/Stone.wav");
        break;
//星杯改变
    case 17:
        team=arg[1].toInt();
        howMany=arg[2].toInt();
        if(team)
        {
            red->setGrail(howMany);
            teamArea->update();
        }
        else
        {
            blue->setGrail(howMany);
            teamArea->update();
        }
        break;
//能量改变
    case 18:
        targetID=arg[1].toInt();
        gem=arg[2].toInt();
        crystal=arg[3].toInt();
        player=playerList.at(targetID);
        player->setGem(gem);
        player->setCrystal(crystal);
        playerArea->update();
        break;
//移牌通告
    case 19:
        howMany=arg[1].toInt();
        cardIDList=arg[2].split(",");
        sourceID=arg[3].toInt();
        sourceArea=arg[4].toInt();
        targetID=arg[5].toInt();
        targetArea=arg[6].toInt();
        if(sourceID!=-1)
        {
            switch(sourceArea)
            {
            case 4:
                player=playerList.at(sourceID);
                player->changeHandCardNum(-howMany);
                break;
            case 5:
                cardID=cardIDList[0].toInt();
                card=dataInterface->getCard(cardID);
                playerList[sourceID]->removeBasicStatus(card);
                break;
            case 6:
                player = playerList.at(sourceID);
                player->changeCoverCardNum(-howMany);

                break;
            }
        }
        if(targetID!=-1)
        {
            switch(targetArea)
            {
            case 4:
                player=playerList.at(targetID);
                player->changeHandCardNum(howMany);
                if(targetID==myID)
                {
                    for(i=0;i<howMany;i++)
                    {
                        cardID=cardIDList[i].toInt();
                        card=dataInterface->getCard(cardID);
                        dataInterface->addHandCard(card);
                    }
                }
                break;
            case 5:
                cardID=cardIDList[0].toInt();
                card=dataInterface->getCard(cardID);
                cardName=card->getName();
                if(cardName==QStringLiteral("中毒"))
                    playerList[targetID]->addBasicStatus(0,card);
                if(cardName==QStringLiteral("虚弱"))
                    playerList[targetID]->addBasicStatus(1,card);
                if(cardName==QStringLiteral("圣盾")||card->getSpecialityList().contains(QStringLiteral("天使之墙")))
                    playerList[targetID]->addBasicStatus(2,card);
                if(card->getType()=="attack"&&card->getProperty()==QStringLiteral("幻"))
                    playerList[targetID]->addBasicStatus(3,card);
                if(card->getSpecialityList().contains(QStringLiteral("威力赐福")))
                    playerList[targetID]->addBasicStatus(4,card);
                if(card->getSpecialityList().contains(QStringLiteral("迅捷赐福")))
                    playerList[targetID]->addBasicStatus(5,card);
                QSound::play("sound/Equip.wav");
                break;
            case 6:
                player=playerList.at(targetID);
                player->changeCoverCardNum(howMany);
                break;
            }
        }
        if(sourceID==-1){
            switch(sourceArea)
            {
            case 1:
                teamArea->changeLeftCardNum(-howMany);
                break;
            case 2:

            case 3:
                teamArea->changeDroppedCardNum(-howMany);
                break;
            }
            teamArea->update();
        }
        if(targetID==-1)
            if(targetArea==2||targetArea==3){
                teamArea->changeDroppedCardNum(howMany);
                teamArea->update();
            }

        playerArea->update();
        break;
//物伤通告
    case 20:
        targetID=arg[1].toInt();
        sourceID=arg[3].toInt();
        gui->logAppend(playerList[sourceID]->getRoleName()+QStringLiteral("对")+playerList[targetID]->getRoleName()+QStringLiteral("造成了")+arg[2]+QStringLiteral("点攻击伤害"));
        break;
//法术通告
    case 21:
        targetID=arg[1].toInt();
        sourceID=arg[3].toInt();
        if(sourceID!=targetID)
            playerArea->drawLineBetween(sourceID,targetID);
        gui->logAppend(playerList[sourceID]->getRoleName()+QStringLiteral("对")+playerList[targetID]->getRoleName()+QStringLiteral("使用的")+arg[4]+QStringLiteral("造成了")+arg[2]+QStringLiteral("点法术伤害"));
        break;
//虚弱询问
    case 22:
        targetID=arg[1].toInt();
        gui->logAppend(QStringLiteral("等待")+playerList[targetID]->getRoleName()+QStringLiteral("虚弱响应"));
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
            tipArea->setMsg(QStringLiteral("你被虚弱了,【确定】跳过行动阶段,【取消】摸")+arg[2]+QStringLiteral("张牌"));
        }
        break;
//虚弱结果
    case 24:
        targetID=arg[1].toInt();
        if(arg[2]=="0")
            gui->logAppend(playerList[targetID]->getRoleName()+QStringLiteral("选择跳过行动阶段"));
        else
            gui->logAppend(playerList[targetID]->getRoleName()+QStringLiteral("选择摸")+arg[3]+QStringLiteral("张牌挣脱"));
        break;
//圣盾移除通告
    case 25:
        targetID=arg[1].toInt();
        gui->logAppend(playerList[targetID]->getRoleName()+QStringLiteral("的圣盾被移除"));
        break;
//魔弹询问
    case 26:
        int nextID;
        targetID=arg[1].toInt();
        sourceID=arg[2].toInt();
        howMany=arg[3].toInt();
        nextID=arg[4].toInt();
        gui->logAppend(playerList[sourceID]->getRoleName()+QStringLiteral("对")+playerList[targetID]->getRoleName()+QStringLiteral("使用了魔弹，目前伤害为：")+arg[3]+QStringLiteral("点"));
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
//卡牌通告
    case 28:
        cardID=arg[1].toInt();
        targetID=arg[2].toInt();
        sourceID=arg[3].toInt();
        flag=arg[4];
        card=dataInterface->getCard(cardID);

        if(targetID!=-1 && targetID!=sourceID)
            playerArea->drawLineBetween(sourceID,targetID);

        if(flag=="1"){
            cards.clear();
            cards<<card;
            showArea->showCards(cards);
        }
        if(card->getElement()!="light")
            gui->logAppend(playerList[sourceID]->getRoleName()+QStringLiteral("对")+playerList[targetID]->getRoleName()+QStringLiteral("使用了<font color=\'orange\'>")+card->getInfo()+"</font>");
        else
            gui->logAppend(playerList[sourceID]->getRoleName()+"使用<font color=\'orange\'>"+card->getInfo()+"</font>抵御");
        break;
//行动阶段 flag 0-所有行动，1-攻击行动，2-法术行动，3-特殊行动，4-攻击或法术行动
    case 29:
        targetID=arg[1].toInt();
        flag=arg[2];
        if(targetID!=myID)
        {
            isMyTurn=0;
            gui->setEnable(0);
            actionFlag=-1;
        }
        else
        {
            gui->setEnable(1);
            if(flag=="0")
                myRole->normal();
            else if(flag=="1")
                myRole->attackAction();
            else if(flag=="2")
                myRole->magicAction();
            else if(flag=="4")
                myRole->attackOrMagic();
            if(arg[3]=="1")
                decisionArea->enable(3);
        }
        break;
//无法行动公告
    case 31:
        targetID=arg[1].toInt();
        msg=playerList[targetID]->getRoleName()+QStringLiteral("宣告无法行动");
        gui->logAppend(msg);
        break;
//治疗改变
    case 32:
        targetID=arg[1].toInt();
        howMany=arg[2].toInt();
        i=playerList[targetID]->getCrossNum();
        playerList[targetID]->setCrossNum(howMany);
        if((howMany=i-howMany) >0)
        {
           msg=playerList[targetID]->getRoleName()+QStringLiteral("治疗减少")+QString::number(howMany);
           gui->logAppend(msg);
        }
        playerArea->update();
        QSound::play("sound/Cure.wav");
        break;
//治疗询问
    case 33:
        targetID=arg[1].toInt();
        howMany=arg[2].toInt();
        flag=arg[3];
        gui->reset();
        if(targetID==myID)
            myRole->cure(playerList[myID]->getCrossNum(),howMany,flag.toInt(),arg[4].toInt());
        break;
//技能响应询问
    case 35:
        targetID=arg[1].toInt();
        flag=arg[2];
        msg=QStringLiteral("等待")+playerList[targetID]->getRoleName()+flag+QStringLiteral("响应");
        gui->logAppend(msg);
        if(targetID==myID)
        {
            gui->setEnable(1);            
            myRole->askForSkill(flag);
        }
        else
            gui->setEnable(0);
        break;
//信息通告
    case 38:
        gui->logAppend(arg[1]);
        if(arg[1].contains(QStringLiteral("发动")))
            QSound::play("sound/Skill.wav");
        break;
//角色形态转换通知
    case 39:
        targetID=arg[1].toInt();
        flag=arg[2];
        if(flag=="0")
        {
            playerList[targetID]->setTap(0);
            msg=playerList[targetID]->getRoleName()+QStringLiteral("返回")+arg[3];
        }
        else
        {
            playerList[targetID]->setTap(1);
            msg=playerList[targetID]->getRoleName()+QStringLiteral("进入")+arg[3];
        }
        playerArea->update();
        gui->logAppend(msg);
        break;
//手牌上限变更通知
    case 40:
        targetID=arg[1].toInt();
        howMany=arg[2].toInt();
        playerList[targetID]->setHandCardsMax(howMany);
        playerArea->update();
        break;
//获得手牌通告
    case 41:
        targetID=arg[1].toInt();
        flag=arg[2];
        howMany=arg[3].toInt();
        gui->logAppend(playerList[targetID]->getRoleName()+QStringLiteral("获得")+arg[3]+QStringLiteral("张手牌"));

        if(targetID==myID)
        {
            cardIDList=arg[4].split(',');
            for(i=0;i<howMany;i++)
            {
                cardID=cardIDList[i].toInt();
                card=dataInterface->getCard(cardID);
                dataInterface->addHandCard(card);
            }
        }
        break;
//额外行动询问
    case 42:
        targetID=arg[1].toInt();
        gui->logAppend(QStringLiteral("等待")+playerList[targetID]->getRoleName()+QStringLiteral("额外行动响应"));
        if(targetID==myID)
        {
            gui->setEnable(1);
            myRole->additionalAction();
        }
        else
            gui->setEnable(0);
        break;
//专属变更
    case 43:
        targetID=arg[1].toInt();
        msg=arg[2];
        playerList.at(targetID)->setSpecial(msg.toInt(),arg[3].toInt());
        playerArea->update();
        break;
//标记变更
    case 45:
        targetID=arg[1].toInt();
        flag=arg[2];
        howMany=arg[3].toInt();
        playerList.at(targetID)->setToken(flag.toInt(),howMany);
        playerArea->update();
        break;
//盖牌通告
    case 48:
        targetID=arg[1].toInt();
        howMany=arg[2].toInt();
        dir = arg[4].toInt();
        show = arg[5].toInt();
        cardIDList=arg[3].split(',');

        if(targetID==myID)
        {

            for(i=0;i<howMany;i++)
            {
                cardID=cardIDList[i].toInt();
                card=dataInterface->getCard(cardID);
                if(dir == 0)
                    dataInterface->addCoverCard(card);
                else
                    dataInterface->removeCoverCard(card);
            }
        }

        if(dir == 0)
            gui->logAppend(arg[2]+QStringLiteral("张牌加入玩家") + playerList[targetID]->getRoleName()+QStringLiteral("盖牌区"));
        else
        {
            if(show == 0)
                gui->logAppend(QStringLiteral("玩家") + playerList[targetID]->getRoleName() + QStringLiteral("移除") + arg[2] + QStringLiteral("张盖牌"));
            else
            {
                QList<Card*> cards;

                QString log = QStringLiteral("玩家") + playerList[targetID]->getRoleName() + QStringLiteral("移除") + arg[2] + QStringLiteral("张盖牌:<font color=\'orange\'>");
                for(int i = 0;i < howMany;i++)
                {
                    cards << dataInterface->getCard(cardIDList[i].toInt());
                    log += cards.at(i)->getInfo();
                }
                log +="</font>";
                gui->logAppend(log);
                showArea->showCards(cards);
            }
        }
        break;
//盖牌弃牌
    case 49:
        targetID=arg[1].toInt();
        howMany=arg[2].toInt();
        flag=arg[3];
        msg=playerList[targetID]->getRoleName()+QStringLiteral("需要弃")+arg[2]+QStringLiteral("张盖牌");
        if(flag=="y")
            gui->logAppend(msg+QStringLiteral("(明弃)"));
        else
            gui->logAppend(msg+QStringLiteral("(暗弃)"));
        if(targetID!=myID)
        {
            gui->setEnable(0);
        }
        else
        {
            gui->setEnable(1);
            gui->reset();
            dropCover(howMany);
            tipArea->setMsg(QStringLiteral("你需要弃")+arg[2]+QStringLiteral("张盖牌"));
        }
        break;
//聊天
    case 59:
        gui->chatAppend(arg[1].toInt(),arg[2]);
        break;
//天使祝福
    case 750:
        targetID=arg[1].toInt();
        howMany=arg[2].toInt();
        msg=QStringLiteral("等待")+playerList[targetID]->getRoleName()+QStringLiteral("天使祝福（给牌）响应");
        gui->logAppend(msg);
        if(targetID!=myID)
        {
            isMyTurn=0;
            gui->setEnable(0);
        }
        else
        {
            gui->setEnable(1);
            TianShiZhuFu(howMany);
        }
        break;
//魔爆冲击（弃牌）
    case 850:
        targetID=arg[1].toInt();
        msg=QStringLiteral("等待")+playerList[targetID]->getRoleName()+QStringLiteral("魔爆冲击（弃牌）响应");
        gui->logAppend(msg);
        if(targetID!=myID)
        {
            isMyTurn=0;
            gui->setEnable(0);
        }
        else
        {
            gui->setEnable(1);
            MoBaoChongJi();
        }
        break;
//充盈（弃牌）
    case 2950:
        targetID=arg[1].toInt();
        int color=arg[2].toInt();
        msg=QStringLiteral("等待")+playerList[targetID]->getRoleName()+QStringLiteral("充盈（弃牌）响应");
        gui->logAppend(msg);
        if(targetID!=myID)
        {
            isMyTurn=0;
            gui->setEnable(0);
        }
        else
        {
            gui->setEnable(1);
            ChongYing(color);
        }
        break;    
    }
}


