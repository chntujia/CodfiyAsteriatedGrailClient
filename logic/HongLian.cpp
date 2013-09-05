#include "HongLian.h"

HongLian::HongLian()
{
    makeConnection();
    setMyRole(this);
    Button *xingHongShiZi;
    xingHongShiZi = new Button(3,QStringLiteral("腥红十字"));
    buttonArea->addButton(xingHongShiZi);
    connect(xingHongShiZi,SIGNAL(buttonSelected(int)),this,SLOT(XingHongShiZi()));
}

void HongLian::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    QList<Card*> handcards=dataInterface->getHandCards();
    int magic = 0;
    for(int i=0; i<handcards.size();i++)
    {
        if(handcards[i]->getType() == QStringLiteral("magic"))
            magic++;
    }
    if(magic>1&&myself->getToken(0)>0&&myself->getEnergy()>0)
        buttonArea->enable(3);
    unactionalCheck();
}

void HongLian::XingHongShengYue()
{
    state = 36;
    tipArea->setMsg(QStringLiteral("是否发动腥红圣约？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void HongLian::XueXingDaoYan1()
{
    state = 2811;
    gui->reset();
    Player* myself=dataInterface->getMyself();

    tipArea->setMsg(QStringLiteral("【血腥祷言】请选择分给第一人的治疗"));
    for(int i = 1;i<=myself->getCrossNum();i++)
        tipArea->addBoxItem(QString::number(i));
    tipArea->showBox();

    playerArea->enableMate();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void HongLian::XueXingDaoYan2()
{
    state = 2812;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    Player* myself=dataInterface->getMyself();
    tipArea->setMsg(QStringLiteral("【血腥祷言】请选择分给第二人的治疗或直接确认"));
    for(int i = 1;i<=(myself->getCrossNum()-cross[0]);i++)
        tipArea->addBoxItem(QString::number(i));
    tipArea->showBox();
    playerArea->enableMate();
    playerArea->disablePlayerItem(dst[0]);
    playerArea->setQuota(1);
    decisionArea->enable(1);
    decisionArea->enable(0);
}

void HongLian::ShaLuShengYan()
{
    state = 36;
    tipArea->setMsg(QStringLiteral("是否发动杀戮盛宴？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void HongLian::XingHongShiZi()
{
    state = 2803;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();    

    handArea->setQuota(2);
    handArea->enableMagic();
    playerArea->setQuota(1);
    decisionArea->disable(0);
    decisionArea->enable(1);
}

void HongLian::cardAnalyse()
{
    Role::cardAnalyse();
    switch (state)
    {
    case 2803:
        playerArea->enableAll();
        break;
    }
}

void HongLian::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    static QString command;
    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();
    Player* myself=dataInterface->getMyself();

    switch(state)
    {
    //额外行动询问
    case 42:
        text=tipArea->getBoxCurrentText();
        switch (text[0].digitValue()){
        case 1:
            emit sendCommand("2802;"+QString::number(myID)+";");
            attackOrMagic();
            break;
        }
        break;
    case 2811:
        command = "2801;1;";
        command+=QString::number(selectedPlayers[0]->getID())+";"+tipArea->getBoxCurrentText()+";";
        dst[0] = selectedPlayers[0]->getID();
        cross[0] = tipArea->getBoxCurrentText().toInt();
        if(myself->getCrossNum()>cross[0])
            XueXingDaoYan2();
        else
        {
            command +="-1;0;";
            start = true;
            emit sendCommand(command);
        }
        break;
    case 2812:
        if(selectedPlayers.size()>0)
            command+=QString::number(selectedPlayers[0]->getID())+";"+tipArea->getBoxCurrentText()+";";
        else
            command+="-1;0;";
        start = true;
        emit sendCommand(command);
        gui->reset();
        break;
    case 2803:
        command = "2803;";
        command+=QString::number(selectedPlayers[0]->getID())+";"+QString::number(myID)+";";
        foreach(Card*ptr,selectedCards){
            command+=QString::number(ptr->getID())+";";
            dataInterface->removeHandCard(ptr);
        }
        gui->reset();
        emit sendCommand(command);
        break;
    }
}

void HongLian::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
    case 2811:
        command = "2801;0;-1;0;-1;0;";
        emit sendCommand(command);
        break;
    case 2812:
        XueXingDaoYan1();
        break;
    case 2803:
        if(actionFlag==0)
            normal();
        else if(actionFlag==4)
            attackOrMagic();
        break;
    }
}
void HongLian::attackOrMagic()
{
    Role::attackOrMagic();
    Player* myself=dataInterface->getMyself();
    QList<Card*> handcards=dataInterface->getHandCards();
    int magic = 0;
    for(int i=0; i<handcards.size();i++)
    {
        if(handcards[i]->getType() == QStringLiteral("magic"))
            magic++;
    }
    if(magic>1&&myself->getToken(0)>0&&myself->getEnergy()>0)
        buttonArea->enable(3);
}

void HongLian::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==QStringLiteral("腥红圣约"))
        XingHongShengYue();
    else if(skill==QStringLiteral("杀戮盛宴"))
        ShaLuShengYan();
    else if(skill==QStringLiteral("血腥祷言"))
        XueXingDaoYan1();
}

void HongLian::additionalAction()
{
    Role::additionalAction();
    Player* myself=dataInterface->getMyself();
    if(myself->getEnergy()>0 && myself->getTap())
        tipArea->addBoxItem(QStringLiteral("1.戒骄戒躁"));
}

