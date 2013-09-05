#include "GeDouJia.h"

GeDouJia::GeDouJia()
{
    makeConnection();
    setMyRole(this);
}

void GeDouJia::CangYanXuLi()
{
    Player* myself=dataInterface->getMyself();
    state=2001;
    decisionArea->enable(0);
    decisionArea->enable(1);
    if(myself->getTap()==0)
        baiShiUsed=false;
    tipArea->setMsg(QStringLiteral("请选择要发动的技能："));
    if(myself->getToken(0)<6 && !baiShiUsed)
        tipArea->addBoxItem(QStringLiteral("1.蓄力一击"));
    if(myself->getToken(0)<6 && baiShiUsed)
        tipArea->addBoxItem(QStringLiteral("1.蓄力一击，且退出【百式幻龙拳】"));
    if(myself->getToken(0)>0)
        tipArea->addBoxItem(QStringLiteral("2.苍炎之魂"));
    tipArea->showBox();
}

void GeDouJia::NianDan1()
{
    state=2002;
    tipArea->setMsg(QStringLiteral("是否发动念弹？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void GeDouJia::NianDan2()
{
    state=2052;

    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    playerArea->enableEnemy();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void GeDouJia::BaiShiHuanLongQuan()
{
    state=2003;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动百式幻龙拳？"));
    QList<Card*> handcards=dataInterface->getHandCards();
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

void GeDouJia::DouShenTianQu()
{
    state=2004;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动斗神天驱？如是，请选择要弃的牌"));
    QList<Card*> handcards=dataInterface->getHandCards();
    cardReady=false;
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
    {

        if(n<4)
            decisionArea->enable(0);
        else{
            handArea->enableAll();
            handArea->setQuota(n-3);
            cardReady=true;
            }
    }
}

void GeDouJia::cardAnalyse()
{
    tipArea->reset();
    Role::cardAnalyse();
    QList<Card*> handcards=dataInterface->getHandCards();
    switch(state)
    {
    case 2004:
        if(cardReady)
            decisionArea->enable(0);
        break;
    }
}

void GeDouJia::playerAnalyse()
{
    Role::playerAnalyse();
    Player*myself = dataInterface->getMyself();
    QList<Card*> selectedCards=handArea->getSelectedCards();
    QList<Player*>selectedPlayers=playerArea->getSelectedPlayers();
    if(myself->getTap()==0){
        baiShiUsed=false;
        baiShiTargetID=-1;
    }
    switch(state)
    {
    case 1:
    case 10:
    case 12:
        if(baiShiUsed)
        {
            tipArea->reset();
            if(selectedCards[0]->getType()=="magic")
            {
                tipArea->setMsg(QStringLiteral("这会退出【百式幻龙拳】，是否确定？"));
            }
            else if(baiShiFirst)
            {
                baiShiTargetID=selectedPlayers[0]->getID();
            }
            else if(selectedPlayers[0]->getID()!=baiShiTargetID)
            {
                tipArea->setMsg(QStringLiteral("这会退出【百式幻龙拳】，是否确定？"));
            }
            decisionArea->enable(0);
        }
        break;
    }
}

void GeDouJia::buy()
{
    Player*myself = dataInterface->getMyself();
    if(myself->getTap()==0){
        baiShiUsed=false;
        baiShiTargetID=-1;
    }
    if(!baiShiUsed)
        Role::buy();
    else{
        Team*team=dataInterface->getMyTeam();

        int energy=team->getEnergy();

        state=4;
        decisionArea->enable(0);
        decisionArea->enable(1);
        handArea->reset();
        playerArea->reset();
        tipArea->reset();

        if(energy<4)
            tipArea->setMsg(QStringLiteral("你将会退出【百式幻龙拳】,执行【购买】"));
        if(energy==4)
        {
            tipArea->setMsg(QStringLiteral("你将会退出【百式幻龙拳】,执行【购买】"));
            tipArea->addBoxItem(QStringLiteral("1.一个宝石"));
            tipArea->addBoxItem(QStringLiteral("2.一个水晶"));
            tipArea->showBox();
        }
        if(energy==5)
        {
            tipArea->setMsg(QStringLiteral("你将会退出【百式幻龙拳】,执行【购买】"));
        }
    }
}

void GeDouJia::synthetize()
{
    Player*myself = dataInterface->getMyself();
    if(myself->getTap()==0){
        baiShiUsed=false;
        baiShiTargetID=-1;
    }
    if(!baiShiUsed)
        Role::synthetize();
    else{
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

        tipArea->setMsg(QStringLiteral("你将会退出【百式幻龙拳】,执行【合成】"));
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
}

void GeDouJia::extract()
{
    Player*myself = dataInterface->getMyself();
    if(myself->getTap()==0){
        baiShiUsed=false;
        baiShiTargetID=-1;
    }
    if(!baiShiUsed)
        Role::extract();
    else{
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

        tipArea->setMsg(QStringLiteral("你将会退出【百式幻龙拳】,执行【提炼】"));
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
}

void GeDouJia::onOkClicked()
{
    Role::onOkClicked();
    QList<Player*>selectedPlayers;
    QList<Card*>selectedCards;

    QString command;
    QString sourceID;
    QString targetID;
    QString text;
    selectedPlayers=playerArea->getSelectedPlayers();
    selectedCards=handArea->getSelectedCards();

    switch(state)
    {
    case 1:
    case 10:
    case 12:
        if(baiShiUsed)
            baiShiFirst=false;
        break;
    case 2001:
        command="2001;1;";
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1')
            command+"0;";
        else
            command+="1;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2002:
        NianDan2();
        break;
    case 2052:
        command="2002;1;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";"+sourceID+";";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2003:
        command="2003;1;";
        baiShiUsed=true;
        baiShiFirst=true;
        start=true;
        emit sendCommand(command);
        gui->reset();
        break;
    case 2004:
        command="2004;1;";
        start=true;
        command+=QString::number(selectedCards.size())+";";
        foreach(Card*ptr,selectedCards){
            command+=QString::number(ptr->getID())+";";
            dataInterface->removeHandCard(ptr);
        }
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void GeDouJia::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
    case 2001:
        command="2001;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2002:
        command="2002;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2052:
        NianDan1();
        break;
    case 2003:
        command="2003;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 2004:
        command="2004;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void GeDouJia::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==QStringLiteral("苍炎蓄力"))
        CangYanXuLi();
    else if(skill==QStringLiteral("念弹"))
        NianDan1();
    else if(skill==QStringLiteral("百式幻龙拳"))
        BaiShiHuanLongQuan();
    else if(skill==QStringLiteral("斗神天驱"))
        DouShenTianQu();
}

