#include "XianZhe.h"

XianZhe::XianZhe()
{
    makeConnection();
    setMyRole(this);

    Button *moDaoFaDian;
    moDaoFaDian=new Button(3,"魔道法典");
    buttonArea->addButton(moDaoFaDian);
    connect(moDaoFaDian,SIGNAL(buttonSelected(int)),this,SLOT(MoDaoFaDian()));

    Button *shengJieFaDian;
    shengJieFaDian=new Button(4,"圣洁法典");
    buttonArea->addButton(shengJieFaDian);
    connect(shengJieFaDian,SIGNAL(buttonSelected(int)),this,SLOT(ShengJieFaDian()));
}

void XianZhe::normal()
{
    Role::normal();
    Player *myself=dataInterface->getMyself();

    if(myself->getGem()>0 && myself->getHandCardNum()>1)
    {
        buttonArea->enable(3);
        if(myself->getHandCardNum()>2)
            buttonArea->enable(4);
    }
    unactionalCheck();
}

void XianZhe::FaShuFanTan1()
{
    state=1701;
    tipArea->setMsg(QStringLiteral("是否发动法术反弹？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void XianZhe::FaShuFanTan2()
{
    state=1751;

    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    handArea->enableAll();
    handArea->setQuota(2,7);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void XianZhe::MoDaoFaDian()
{
    state=1702;

    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    handArea->enableAll();
    handArea->setQuota(2,7);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void XianZhe::ShengJieFaDian()
{
    state=1703;

    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    handArea->enableAll();
    handArea->setQuota(3,7);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void XianZhe::cardAnalyse()
{
    Role::cardAnalyse();
    QList<Card*> selectedCards=handArea->getSelectedCards();

    switch(state)
    {
    case 1751:
        cardReady=false;
        if(selectedCards.size()<2)
            decisionArea->disable(0);
        else
        {
            foreach(Card*ptr,selectedCards)
                for(int i=0;i<selectedCards.size();i++){
                    if(ptr->getElement()!=selectedCards[i]->getElement() && ptr->getID()!=selectedCards[i]->getID()){
                        playerArea->reset();
                        decisionArea->disable(0);
                        cardReady=true;
                        break;
                    }
                }
            if(!cardReady)
                playerArea->enableAll();
        }
        break;
    case 1702:
        cardReady=false;
        if(selectedCards.size()<2)
            decisionArea->disable(0);
        else
        {
            foreach(Card*ptr,selectedCards)
                for(int i=0;i<selectedCards.size();i++){
                    if(ptr->getElement()==selectedCards[i]->getElement() && ptr->getID()!=selectedCards[i]->getID()){
                        playerArea->reset();
                        decisionArea->disable(0);
                        cardReady=true;
                        break;
                    }
                }
            if(!cardReady)
                playerArea->enableAll();
        }
        break;
    case 1703:
        cardReady=false;
        if(selectedCards.size()<3)
            decisionArea->disable(0);
        else
        {
            foreach(Card*ptr,selectedCards)
                for(int i=0;i<selectedCards.size();i++){
                    if(ptr->getElement()==selectedCards[i]->getElement() && ptr->getID()!=selectedCards[i]->getID()){
                        playerArea->reset();
                        decisionArea->disable(0);
                        cardReady=true;
                        break;
                    }
                }
            if(!cardReady){
                playerArea->enableAll();
                decisionArea->enable(0);
            }
        }
        break;
    }
}

void XianZhe::playerAnalyse()
{
    Role::playerAnalyse();
    QList<Card*> selectedCards=handArea->getSelectedCards();
    switch(state)
    {
    case 1751:
    case 1702:
        playerArea->setQuota(1);
        cardReady=false;
        break;
    case 1703:
        playerArea->setQuota(0,selectedCards.size()-2);
        cardReady=false;
        break;
    }
}

void XianZhe::onOkClicked()
{
    Role::onOkClicked();
    QList<Player*>selectedPlayers;
    QList<Card*>selectedCards;

    QString command;
    QString sourceID;
    QString targetID;
    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    switch(state)
    {
    case 1701:
        FaShuFanTan2();
        break;
    case 1751:
        command="1701;1;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";"+sourceID+";"+QString::number(selectedCards.size())+";";
        foreach(Card*ptr,selectedCards){
            command+=QString::number(ptr->getID())+";";
            dataInterface->removeHandCard(ptr);
        }
        emit sendCommand(command);
        gui->reset();
        break;
    case 1702:
        command="1702;";
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";"+sourceID+";"+QString::number(selectedCards.size())+";";
        foreach(Card*ptr,selectedCards){
            command+=QString::number(ptr->getID())+":";
            dataInterface->removeHandCard(ptr);
        }
        command+=";";
        emit sendCommand(command);
        gui->reset();
        break;
    case 1703:
        command="1703;";
        sourceID=QString::number(myID);
        command+=sourceID+";"+QString::number(selectedCards.size())+";"+QString::number(selectedPlayers.size())+";";
        foreach(Card*ptr,selectedCards){
            command+=QString::number(ptr->getID())+":";
            dataInterface->removeHandCard(ptr);
        }
        command+=";";
        foreach(Player*ptr,selectedPlayers)
            command+=QString::number(ptr->getID())+":";
        command+=";";
        emit sendCommand(command);
        gui->reset();
        break;
    }
}

void XianZhe::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
    case 1701:
        command="1701;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 1751:
        FaShuFanTan1();
        break;
    case 1702:
    case 1703:
        normal();
        break;
    }
}

void XianZhe::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==QStringLiteral("法术反弹"))
        FaShuFanTan1();
}


