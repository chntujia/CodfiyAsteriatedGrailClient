#include "FengYin.h"

FengYin::FengYin()
{
    makeConnection();
setMyRole(this);
    connect(playerArea,SIGNAL(playerUnready()),this,SLOT(onUnready()));

    Button *fengYinFaShu,*wuXiShuFu,*fengYinPoSui;
    fengYinFaShu=new Button(3,QStringLiteral("封印法术"));
    buttonArea->addButton(fengYinFaShu);
    connect(fengYinFaShu,SIGNAL(buttonSelected(int)),this,SLOT(FengYinFaShu()));

    wuXiShuFu=new Button(4,QStringLiteral("五系束缚"));
    buttonArea->addButton(wuXiShuFu);
    connect(wuXiShuFu,SIGNAL(buttonSelected(int)),this,SLOT(WuXiShuFu()));

    fengYinPoSui=new Button(5,QStringLiteral("封印破碎"));
    buttonArea->addButton(fengYinPoSui);
    connect(fengYinPoSui,SIGNAL(buttonSelected(int)),this,SLOT(FengYinPoSui()));
}

void FengYin::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
//封印法术
    foreach(Card*ptr, dataInterface->getHandCards())
        if (ptr->getType()=="attack"&&ptr->getProperty()==QStringLiteral("幻"))
            buttonArea->enable(3);
//五系束缚
    if(myself->getEnergy()>0)
        buttonArea->enable(4);
//封印破碎
    if(myself->getEnergy()>0)
        foreach(Player*ptr,dataInterface->getPlayerList())
            if(ptr->hasStatus()){
                buttonArea->enable(5);
                break;
            }
    unactionalCheck();
}

void FengYin::FengYinFaShu()
{
    state=401;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableProperty(QStringLiteral("幻"));
    handArea->disableMagic();
}

void FengYin::WuXiShuFu()
{
    state=402;
    int gem,crystal;
    Player*myself=dataInterface->getMyself();

    gem=myself->getGem();
    crystal=myself->getCrystal();

    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->disable(0);
    playerArea->setQuota(1);
    playerArea->enableEnemy();

    tipArea->setMsg(QStringLiteral("请选择使用的能量："));
    if(crystal>=1)
        tipArea->addBoxItem(QStringLiteral("1.水晶"));
    if(gem>=1)
        tipArea->addBoxItem(QStringLiteral("2.宝石"));

    tipArea->showBox();
}

void FengYin::FengYinPoSui()
{
    state=403;
    int gem,crystal;
    Player*myself=dataInterface->getMyself();
    QList<Player*>players=dataInterface->getPlayerList();

    gem=myself->getGem();
    crystal=myself->getCrystal();

    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->disable(0);
    playerArea->setQuota(1);
    for(int i=0;i<players.size();i++)
        if(players[i]->hasStatus())
            playerArea->enablePlayerItem(i);

    tipArea->setMsg(QStringLiteral("请选择使用的能量："));
    if(crystal>=1)
        tipArea->addBoxItem(QStringLiteral("1.水晶"));
    if(gem>=1)
        tipArea->addBoxItem(QStringLiteral("2.宝石"));

    tipArea->showBox();
}

void FengYin::onUnready()
{
    switch(state)
    {
    case 403:
        tipArea->reset();
        FengYinPoSui();
        break;
    }
}

void FengYin::additionalAction()
{
    Role::additionalAction();
    if(usedMagic)
        tipArea->addBoxItem(QStringLiteral("1.法术激荡"));
}

void FengYin::cardAnalyse()
{
    Role::cardAnalyse();

    switch (state)
    {
    case 401:
        QList<Player*>players=dataInterface->getPlayerList();
        QList<Card*>selectedCards=handArea->getSelectedCards();
        playerArea->enableEnemy();
        for(int i=0;i<players.size();i++)
            for(int j=0;j<players[i]->getBasicStatus().size();j++)
                if(players[i]->checkBasicStatus(selectedCards[0]->getSpecialityList().at(0)))
                    playerArea->disablePlayerItem(i);
        break;
    }
}
void FengYin::playerAnalyse()
{
    QString text;
    switch (state)
    {
    case 403:
        text=tipArea->getBoxCurrentText();
        if(text[0].digitValue()==1)
            usedGem=0;
        else
            usedGem=1;
        tipArea->showStatus(playerArea->getSelectedPlayers().at(0)->getID());
        break;
    default:
        decisionArea->enable(0);
        break;
    }
}

void FengYin::onOkClicked()
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
        if(text[0].digitValue()==1){
            emit sendCommand("404;"+QString::number(myID)+";");            
            attackAction();
        }
        break;
//封印法术
    case 401:
        command="401;";
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=cardID+";"+targetID+";"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        usedMagic=true;
        emit sendCommand(command);
        gui->reset();
        break;
//五系束缚
    case 402:
        command="402;";
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1')
            command+="0;";
        else
            command+="1;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";"+sourceID+";";
        usedMagic=true;
        emit sendCommand(command);
        gui->reset();
        break;
//封印破碎
    case 403:
        command="403;";
        if(usedGem)
            command+="1;";
        else
            command+="0;";
        command+=QString::number(selectedPlayers[0]->getID())+";";
        command+=QString::number(myID)+";";
        command+=QString::number(tipArea->getSelectedCardID())+";";
        usedMagic=true;
        emit sendCommand(command);
        gui->reset();
        break;
    }
}


void FengYin::onCancelClicked()
{
    Role::onCancelClicked();
    switch(state)
    {
//封印法术
    case 401:
//五系束缚
    case 402:
//封印破碎
    case 403:
        normal();
        break;
    }
}
