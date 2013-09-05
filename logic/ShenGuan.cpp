#include "ShenGuan.h"

ShenGuan::ShenGuan()
{
    makeConnection();
    setMyRole(this);
    Button *shenShengQiFu, *shuiZhiShenLi, *shenShengLingYu;
    shenShengQiFu = new Button(3,QStringLiteral("神圣祈福"));
    buttonArea->addButton(shenShengQiFu);
    connect(shenShengQiFu,SIGNAL(buttonSelected(int)),this,SLOT(ShenShengQiFu()));

    shuiZhiShenLi = new Button(4,QStringLiteral("水之神力"));
    buttonArea->addButton(shuiZhiShenLi);
    connect(shuiZhiShenLi,SIGNAL(buttonSelected(int)),this,SLOT(ShuiZhiShenLi1()));

    shenShengLingYu = new Button(5,QStringLiteral("神圣领域"));
    buttonArea->addButton(shenShengLingYu);
    connect(shenShengLingYu,SIGNAL(buttonSelected(int)),this,SLOT(ShenShengLingYu1()));
}

void ShenGuan::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    QList<Card*> handcards=dataInterface->getHandCards();
    int qiFu = 0;
    for(int i=0; i<handcards.size();i++)
    {
        if(handcards[i]->getType() == QStringLiteral("magic"))
            qiFu++;
    }
    if(qiFu>1)
        buttonArea->enable(3);
    if(handArea->checkElement("water"))
        buttonArea->enable(4);
    if(myself->getEnergy()>0)
        buttonArea->enable(5);
    unactionalCheck();
}

void ShenGuan::ShenShengQiShi()
{
    state = 36;
    tipArea->setMsg(QStringLiteral("是否发动神圣启示？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void ShenGuan::ShenShengQiFu()
{
    state = 1502;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    handArea->setQuota(2);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableMagic();
}

void ShenGuan::ShuiZhiShenLi1()
{
    state = 1531;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("water");
}

void ShenGuan::ShuiZhiShenLi2()
{
    state = 1532;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    tipArea->setMsg("请给目标角色一张牌");

    handArea->setQuota(1);
    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableAll();
}

void ShenGuan::ShenShengQiYue1()
{
    state = 1541;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动神圣契约"));
    QList<Card*> handcards=dataInterface->getHandCards();
    bool flag=true;
    if(handcards.size()==1 && handcards.at(0)->getType()=="light" && dataInterface->getMyself()->getEnergy()==1)
        flag=false;
    if(flag)
        decisionArea->enable(0);
    decisionArea->enable(1);
}

void ShenGuan::ShenShengQiYue2()
{
    state = 1542;
    Player* myself=dataInterface->getMyself();
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    int cross = myself->getCrossNum();
    if (cross>4)
        cross = 4;
    for(;cross>0;cross--)
        tipArea->addBoxItem(QString::number(cross));
    tipArea->setMsg("请选择要转移的治疗数目");
    tipArea->showBox();

    playerArea->setQuota(1);
    playerArea->enableMate();

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void ShenGuan::ShenShengLingYu1()
{
    state = 1551;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->enable(0);

    tipArea->setMsg(QStringLiteral("请先选择一项："));
    if(dataInterface->getMyself()->getCrossNum()>0)
        tipArea->addBoxItem(QStringLiteral("1.（移除1治疗）对目标角色造成2点法术伤害"));
    tipArea->addBoxItem(QStringLiteral("2.增加2治疗，目标队友增加1治疗"));
    tipArea->showBox();
}

void ShenGuan::ShenShengLingYu2()
{
    state = 1552;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);

    QList<Card*> handcards=dataInterface->getHandCards();
    if(handcards.size()>1)
        handArea->setQuota(2);
    else if(handcards.size()==1)
        handArea->setQuota(1);
    else
    {
        if(lingYu==1)
            playerArea->enableAll();
        else
            playerArea->enableMate();
    }

    handArea->enableAll();
    decisionArea->enable(1);
    decisionArea->disable(0);
}

void ShenGuan::cardAnalyse()
{
    Role::cardAnalyse();
    switch (state)
    {
    case 1502:
        decisionArea->enable(0);
        break;
    case 1531:
        playerArea->enableMate();
        break;
    case 1532:
        decisionArea->enable(0);
        break;
    case 1552:
        if(lingYu==1)
            playerArea->enableAll();
        else
            playerArea->enableMate();
        break;
    }
}

void ShenGuan::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    static QString command;
    QString cardID;
    QString sourceID;
    QString targetID;
    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    switch(state)
    {
    case 1502:
        command = "1502;";
        foreach(Card*ptr,selectedCards){
            command+=QString::number(ptr->getID())+";";
            dataInterface->removeHandCard(ptr);
        }
        command+=QString::number(myID);
        emit sendCommand(command);
        gui->reset();
        break;
    case 1531:
        command = "1531;";
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";"+cardID+";"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
    case 1532:
        command = "1532;";
        cardID=QString::number(selectedCards[0]->getID());
        command+=cardID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
    case 1541:
        command = "1504;1;";
        ShenShengQiYue2();
        break;
    case 1542:
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        text=tipArea->getBoxCurrentText();
        command += targetID+";"+text+";"+sourceID+";";
        start = true;
        emit sendCommand(command);
        gui->reset();
        break;
    case 1551:
        if(tipArea->getBoxCurrentText()[0]=='1')
            lingYu = 1;
        else
            lingYu = 2;
        ShenShengLingYu2();
        break;
    case 1552:
        command = "1505;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=QString::number(lingYu)+";"+targetID+";";
        foreach(Card*ptr,selectedCards){
            command+=QString::number(ptr->getID())+";";
            dataInterface->removeHandCard(ptr);
        }
        for(int i=0;i<(2-selectedCards.size());i++)
            command+="-1;";
        command+=sourceID+";";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void ShenGuan::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
    case 1502:
    case 1531:
    case 1551:
    case 1552:
        normal();
        break;
    case 1541:
        command = "1504;0;0;0;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 1542:
        gui->reset();
        ShenShengQiYue1();
        break;
    }
}

void ShenGuan::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==QStringLiteral("神圣启示"))
        ShenShengQiShi();
    else if(skill==QStringLiteral("神圣契约"))
        ShenShengQiYue1();
    else if(skill==QStringLiteral("水之神力给牌"))
        ShuiZhiShenLi2();
}
