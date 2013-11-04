#include "SiLing.h"
SiLing::SiLing()
{
    makeConnection();
    setMyRole(this);
    Button *wenYi, *siWangZhiChu, *muBeiYunLuo;
    wenYi = new Button(3,QStringLiteral("瘟疫"));
    buttonArea->addButton(wenYi);
    connect(wenYi,SIGNAL(buttonSelected(int)),this,SLOT(WenYi()));

    siWangZhiChu = new Button(4,QStringLiteral("死亡之触"));
    buttonArea->addButton(siWangZhiChu);
    connect(siWangZhiChu,SIGNAL(buttonSelected(int)),this,SLOT(SiWangZhiChu()));

    muBeiYunLuo = new Button(5,QStringLiteral("墓碑陨落"));
    buttonArea->addButton(muBeiYunLuo);
    connect(muBeiYunLuo,SIGNAL(buttonSelected(int)),this,SLOT(MuBeiYunLuo()));
}

void SiLing::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    QList<Card*> handcards=dataInterface->getHandCards();
    if(handArea->checkElement("earth"))
        buttonArea->enable(3);
    bool siChu = false;
    for(int i=0;i<handcards.size()-1;i++)
        for(int j=i+1;j<handcards.size();j++)
            if(handcards[i]->getElement()==handcards[j]->getElement()){
                siChu = true;
                break;
            }
    if(siChu&&myself->getCrossNum()>1)
        buttonArea->enable(4);
    if(myself->getGem()>0)
        buttonArea->enable(5);
    unactionalCheck();
}

void SiLing::BuXiu()
{
    state = 36;
    tipArea->setMsg(QStringLiteral("是否发动不朽？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void SiLing::WenYi()
{
    state = 1302;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("earth");
}

void SiLing::SiWangZhiChu()
{
    state = 1303;
    Player* myself=dataInterface->getMyself();
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    handArea->setQuota(2,7);
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableAll();

    tipArea->setMsg("请选择移除的治疗，最少为2");
    int cross = myself->getCrossNum();
    for(;cross>1;cross--)
        tipArea->addBoxItem(QString::number(cross));
    tipArea->showBox();
}

void SiLing::MuBeiYunLuo()
{
    state = 1304;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(0);
    decisionArea->enable(1);
}

void SiLing::cardAnalyse()
{
    Role::cardAnalyse();
    bool siChu = true;
    QList<Card*> selectedCards=handArea->getSelectedCards();
    switch (state)
    {
    case 1302:
        decisionArea->enable(0);
        break;
    case 1303:
        foreach(Card*ptr,selectedCards)
            if(ptr->getElement()!=selectedCards[0]->getElement()){
                playerArea->disableAll();
                decisionArea->disable(0);
                siChu = false;
                break;
            }
        if(siChu)
            playerArea->enableAll();
        break;
    }
}

void SiLing::onOkClicked()
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

    network::Action* action;
    network::Respond* respond;

    switch(state)
    {
    case 1302:
        action = newAction(1302);
        action->add_args(selectedCards[0]->getID());
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case 1303:
        action = newAction(1303);
        action->add_dst_ids(selectedPlayers[0]->getID());
        action->add_args(tipArea->getBoxCurrentText().toInt());
        foreach(Card*ptr,selectedCards){
            action->add_args(ptr->getID());
            dataInterface->removeHandCard(ptr);
        }
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case 1304:
        action = newAction(1304);
        action->add_args(1);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    }
}

void SiLing::onCancelClicked()
{
    Role::onCancelClicked();
    switch(state)
    {
    case 1302:
    case 1303:
    case 1304:
        normal();
        break;
    }
}

void SiLing::askForSkill(QString skill)
{
    //Role::askForSkill(skill);
    if(skill==QStringLiteral("不朽"))
        BuXiu();
}
