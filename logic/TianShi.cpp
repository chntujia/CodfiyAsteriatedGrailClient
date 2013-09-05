#include "TianShi.h"

TianShi::TianShi()
{
    makeConnection();
setMyRole(this);

    connect(playerArea,SIGNAL(playerUnready()),this,SLOT(onUnready()));
    connect(handArea,SIGNAL(cardUnready()),this,SLOT(onUnready()));

    Button *fengZhiJieJin,*tianShiZhuFu,*tianShiZhiQiang;
    fengZhiJieJin=new Button(3,QStringLiteral("风之洁净"));
    buttonArea->addButton(fengZhiJieJin);
    connect(fengZhiJieJin,SIGNAL(buttonSelected(int)),this,SLOT(FengZhiJieJin()));

    tianShiZhuFu=new Button(4,QStringLiteral("天使祝福"));
    buttonArea->addButton(tianShiZhuFu);
    connect(tianShiZhuFu,SIGNAL(buttonSelected(int)),this,SLOT(TianShiZhuFu()));

    tianShiZhiQiang=new Button(5,QStringLiteral("天使之墙"));
    buttonArea->addButton(tianShiZhiQiang);
    connect(tianShiZhiQiang,SIGNAL(buttonSelected(int)),this,SLOT(TianShiZhiQiang()));
}
void TianShi::TianShiJiBan()
{
    playerArea->reset();
    tipArea->reset();

    state=707;
    tipArea->setMsg(QStringLiteral("发动天使羁绊"));
    playerArea->setQuota(1);
    playerArea->enableAll();

    decisionArea->disable(1);
    decisionArea->disable(0);
}

void TianShi::FengZhiJieJin()
{
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    state=701;
    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("wind");
}

void TianShi::TianShiZhuFu()
{
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    state=702;
    playerArea->setQuota(1,2);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("water");
}
void TianShi::TianShiZhiQiang()
{
    state=703;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableSpecility(QStringLiteral("天使之墙"));
}

void TianShi::TianShiZhiGe1()
{
    gui->reset();
    int gem,crystal;
    Player*myself=dataInterface->getMyself();

    gem=myself->getGem();
    crystal=myself->getCrystal();
    state=704;
    tipArea->setMsg(QStringLiteral("是否发动天使之歌？"));
    if(crystal>=1)
        tipArea->addBoxItem(QStringLiteral("1.水晶"));
    if(gem>=1)
        tipArea->addBoxItem(QStringLiteral("2.宝石"));

    tipArea->showBox();
    decisionArea->enable(0);
    decisionArea->enable(1);
}
void TianShi::TianShiZhiGe2()
{
    QList<Player*> players=dataInterface->getPlayerList();
    int i;
    state=706;

    for(i=0;i<players.size();i++)
    {
        if(players[i]->hasStatus())
            playerArea->enablePlayerItem(i);
    }
    playerArea->setQuota(1);
    decisionArea->enable(1);
}

void TianShi::ShenZhiBiHu(int reduce)
{
    int gem,crystal;
    Player* myself=dataInterface->getMyself();

    gem=myself->getGem();
    crystal=myself->getCrystal();

    state=705;

    tipArea->reset();
    handArea->reset();
    playerArea->reset();
    decisionArea->enable(0);
    decisionArea->enable(1);
    tipArea->setMsg(QStringLiteral("是否发动神之庇护？"));
    if(reduce>=3)
    {
        if(gem==3)
            tipArea->addBoxItem(QStringLiteral("1.三个宝石"));
        if(crystal==3)
            tipArea->addBoxItem(QStringLiteral("2.三个水晶"));
        if(gem==2&&crystal==1)
            tipArea->addBoxItem(QStringLiteral("3.两个宝石和一个水晶"));
        if(gem==1&&crystal==2)
            tipArea->addBoxItem(QStringLiteral("4.一个宝石和两个水晶"));
    }
    if(reduce>=2)
    {
        if(gem>=2)
            tipArea->addBoxItem(QStringLiteral("5.两个宝石"));
        if(crystal>=2)
            tipArea->addBoxItem(QStringLiteral("6.两个水晶"));
        if(gem>=1&&crystal>=1)
            tipArea->addBoxItem(QStringLiteral("7.一个宝石和一个水晶"));
    }
    if(gem>=1)
        tipArea->addBoxItem(QStringLiteral("8.一个宝石"));
    if(crystal>=1)
        tipArea->addBoxItem(QStringLiteral("9.一个水晶"));
    tipArea->showBox();
}

void TianShi::normal()
{
    Role::normal();
//风之洁净
    if (handArea->checkElement("wind"))
        foreach(Player*ptr,dataInterface->getPlayerList())
            if(ptr->hasStatus()){
                buttonArea->enable(3);
                break;
            }
//天使祝福
    if (handArea->checkElement("water"))
        buttonArea->enable(4);
//天使之墙
    if (handArea->checkSpecility(QStringLiteral("天使之墙")))
        buttonArea->enable(5);
    unactionalCheck();
}
void TianShi::playerAnalyse()
{
    switch (state)
    {
    case 701:
    case 706:
        tipArea->showStatus(playerArea->getSelectedPlayers().at(0)->getID());
        break;
    default:
        decisionArea->enable(0);
        break;
    }

}

void TianShi::cardAnalyse()
{
    Role::cardAnalyse();

    QList<Player*>players=dataInterface->getPlayerList();
    int i;

    switch (state)
    {
    case 701:
        for(i=0;i<players.size();i++)
            if(players[i]->hasStatus())
                playerArea->enablePlayerItem(i);
        break;
    case 702:
        playerArea->enableAll();
        break;
    case 703:
        for(i=0;i<dataInterface->getPlayerMax();i++)
            if(!dataInterface->getPlayerList().at(i)->checkBasicStatus(2))
                playerArea->enablePlayerItem(i);
        break;
    }
}

void TianShi::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    QString command;
    QString cardID;
    QString sourceID;
    QString targetID;
    QString text;
    int n;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    switch(state)
    {
//风之洁净
    case 701:
        command="701;";
        cardID=QString::number(selectedCards[0]->getID());
        command+=cardID+";";
        command+=QString::number(selectedPlayers[0]->getID())+";";
        command+=QString::number(myID)+";";
        command+=QString::number(tipArea->getSelectedCardID())+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
//天使祝福
    case 702:
        command="702;";
        cardID=QString::number(selectedCards[0]->getID());
        command+=cardID+";";
        command+=QString::number(myID)+";";
        n=selectedPlayers.size();
        command+=QString::number(n)+";";
        command+=QString::number(selectedPlayers[0]->getID())+";";
        if(n>1)
            command+=QString::number(selectedPlayers[1]->getID())+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
//天使之墙
    case 703:
        command="703;";
        cardID=QString::number(selectedCards[0]->getID());
        targetID=QString::number(selectedPlayers[0]->getID());
        sourceID=QString::number(myID);
        command+=cardID+";"+targetID+";"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
//天使之歌1
    case 704:
        text=tipArea->getBoxCurrentText();
        switch(text[0].digitValue())
        {
        case 1:
            usedGem=0;
            break;
        case 2:
            usedGem=1;
            break;
        }
        gui->reset();
        TianShiZhiGe2();
        break;
//神之庇护
        /*
            tipArea->addBoxItem(QStringLiteral("1.三个宝石"));
            tipArea->addBoxItem(QStringLiteral("2.三个水晶"));
            tipArea->addBoxItem(QStringLiteral("3.两个宝石和一个水晶"));
            tipArea->addBoxItem(QStringLiteral("4.一个宝石和两个水晶"));
            tipArea->addBoxItem(QStringLiteral("5.两个宝石"));
            tipArea->addBoxItem(QStringLiteral("6.两个水晶"));
            tipArea->addBoxItem(QStringLiteral("7.一个宝石和一个水晶"));
    tipArea->addBoxItem(QStringLiteral("8.一个宝石"));
    tipArea->addBoxItem(QStringLiteral("9.一个水晶"));*/
    case 705:
        command="705;1;";
        text=tipArea->getBoxCurrentText();
        switch(text[0].digitValue())
        {
        case 1:
            command+="3;0;";
            break;
        case 2:
            command+="0;3;";
            break;
        case 3:
            command+="2;1;";
            break;
        case 4:
            command+="1;2;";
            break;
        case 5:
            command+="2;0;";
            break;
        case 6:
            command+="0;2;";
            break;
        case 7:
            command+="1;1;";
            break;
        case 8:
            command+="1;0;";
            break;
        case 9:
            command+="0;1;";
            break;
        }
        emit sendCommand(command);
        gui->reset();
        break;
//天使之歌2
    case 706:
        command="704;1;";
        if(usedGem)
            command+="1;";
        else
            command+="0;";
        command+=QString::number(selectedPlayers[0]->getID())+";";
        command+=QString::number(tipArea->getSelectedCardID())+";";
        emit sendCommand(command);
        gui->reset();
        break;
//天使羁绊
    case 707:
        command="707;";
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void TianShi::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
//风之洁净
    case 701:
//天使祝福
    case 702:
//天使之墙
    case 703:
        gui->reset();
        normal();
        break;
//天使之歌1
    case 704:
        command="704;0;";
        emit sendCommand(command);
        gui->reset();
        break;
//神之庇护
    case 705:
        command="705;0;";
        emit sendCommand(command);
        gui->reset();
        break;
//天使之歌2
    case 706:
        gui->reset();
        TianShiZhiGe1();
        break;
    }
}
void TianShi::onUnready()
{
    switch(state)
    {
    case 701:
        tipArea->reset();
        FengZhiJieJin();
        break;
    case 706:
        tipArea->reset();
        TianShiZhiGe2();
        break;
    }
}
void TianShi::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==QStringLiteral("天使之歌"))
        TianShiZhiGe1();
    else if(skill==QStringLiteral("神之庇护"))
        ShenZhiBiHu(command.split(';').at(3).toInt());
    else if(skill==QStringLiteral("天使羁绊"))
        TianShiJiBan();
}
