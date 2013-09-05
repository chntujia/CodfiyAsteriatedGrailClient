#include "GongNv.h"
GongNv::GongNv()
{
    makeConnection();
setMyRole(this);

    Button *shanGuangXianJing,*juJi;
    shanGuangXianJing=new Button(3,QStringLiteral("闪光陷阱"));
    buttonArea->addButton(shanGuangXianJing);
    connect(shanGuangXianJing,SIGNAL(buttonSelected(int)),this,SLOT(ShanGuangXianJing()));

    juJi=new Button(4,QStringLiteral("狙击"));
    buttonArea->addButton(juJi);
    connect(juJi,SIGNAL(buttonSelected(int)),this,SLOT(JuJi()));
}

void GongNv::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
//闪光陷阱
    if (handArea->checkSpecility(QStringLiteral("闪光陷阱")))
        buttonArea->enable(3);

//狙击
    if(myself->getEnergy()>0)
        buttonArea->enable(4);
    unactionalCheck();
}

void GongNv::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==QStringLiteral("贯穿射击"))
        GuanChuanSheJi();
    else if(skill==QStringLiteral("精准射击"))
        JingZhunSheJi();
}

void GongNv::additionalAction()
{
    Role::additionalAction();
    if(JuJiAdditon)
        tipArea->addBoxItem(QStringLiteral("1.攻击行动（狙击）"));
}

void GongNv::GuanChuanSheJi()
{
    state=301;
    tipArea->setMsg(QStringLiteral("是否发动贯穿射击？"));
    handArea->setQuota(1);
    handArea->enableMagic();
    decisionArea->enable(1);
}

void GongNv::JingZhunSheJi()
{
    state=36;
    tipArea->setMsg(QStringLiteral("是否发动精准射击？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void GongNv::ShanGuangXianJing()
{
    state=302;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableSpecility(QStringLiteral("闪光陷阱"));
}

void GongNv::JuJi()
{
    int gem,crystal;
    Player*myself=dataInterface->getMyself();

    gem=myself->getGem();
    crystal=myself->getCrystal();

    state=303;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->disable(0);
    playerArea->setQuota(1);
    playerArea->enableAll();

    tipArea->setMsg(QStringLiteral("请选择使用的能量："));
    if(crystal>=1)
        tipArea->addBoxItem(QStringLiteral("1.水晶"));
    if(gem>=1)
        tipArea->addBoxItem(QStringLiteral("2.宝石"));

    tipArea->showBox();
}

void GongNv::cardAnalyse()
{
    Role::cardAnalyse();

    switch (state)
    {
    case 301:
        decisionArea->enable(0);
        break;
    case 302:
        playerArea->enableAll();
        break;
    }
}


void GongNv::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    QString command;
    QString cardID;
    QString sourceID;
    QString targetID;
    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    switch(state)
    {
//额外行动询问
    case 42:
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1'){                        
            emit sendCommand("304;"+QString::number(myID)+";");
            JuJiAdditon=false;
            attackAction();
        }
        break;
//贯穿询问
    case 301:
        command="301;1;";
        cardID=QString::number(selectedCards[0]->getID());
        command+=cardID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;   
//闪光陷阱
    case 302:
        command="302;";
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=cardID+";"+targetID+";"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
//狙击
    case 303:
        JuJiAdditon=true;
        command="303;";
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1')
            command+="0;";
        else
            command+="1;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";"+sourceID+";";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}


void GongNv::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
//贯穿询问
    case 301:
        command="301;0;;";
        emit sendCommand(command);
        gui->reset();
        break;
//闪光陷阱
    case 302:
//狙击
    case 303:
        normal();
        break;
    }
}
