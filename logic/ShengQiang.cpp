#include "shengqiang.h"

ShengQiang::ShengQiang()
{
    makeConnection();
setMyRole(this);
    Button *huiYao, *chengJie, *shengGuangQiYu;
    huiYao = new Button(3,QStringLiteral("辉耀"));
    buttonArea->addButton(huiYao);
    connect(huiYao,SIGNAL(buttonSelected(int)),this,SLOT(HuiYao()));

    chengJie = new Button(4,QStringLiteral("惩戒"));
    buttonArea->addButton(chengJie);
    connect(chengJie,SIGNAL(buttonSelected(int)),this,SLOT(ChengJie()));

    shengGuangQiYu = new Button(5,QStringLiteral("圣光祈愈"));
    buttonArea->addButton(shengGuangQiYu);
    connect(shengGuangQiYu,SIGNAL(buttonSelected(int)),this,SLOT(ShengGuangQiYu()));
}

void ShengQiang::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    if(handArea->checkElement("water"))
        buttonArea->enable(3);
    bool chengjieflag = false;
    QList<Player*>players=dataInterface->getPlayerList();
    for(int i=0;i<players.size();i++)
    {
        if(myself->getID()==players[i]->getID())
            continue;
        if(players[i]->getCrossNum()>0)
        {
            chengjieflag = true;
            break;
        }
    }
    if(handArea->checkType("magic")&&chengjieflag)
        buttonArea->enable(4);
    if(myself->getGem()>0)
        buttonArea->enable(5);
    unactionalCheck();
}

void ShengQiang::HuiYao()
{
    state=1001;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("water");
}

void ShengQiang::ChengJie()
{
    state=1002;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableMagic();

}

void ShengQiang::TianQiang()
{
    state=36;
    tipArea->setMsg(QStringLiteral("是否发动天枪？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void ShengQiang::DiQiang()
{
    state=1005;
    tipArea->setMsg(QStringLiteral("请选择发动地枪使用的治疗点数，取消将发动圣击"));
    decisionArea->enable(0);
    decisionArea->enable(1);
    Player* myself=dataInterface->getMyself();
    int cross = myself->getCrossNum();
    if (cross>4)
        cross = 4;
    for(;cross>0;cross--)
        tipArea->addBoxItem(QString::number(cross));
    tipArea->showBox();
}

void ShengQiang::ShengGuangQiYu()
{
    state=1006;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->enable(0);
}

void ShengQiang::cardAnalyse()
{
    Role::cardAnalyse();

    switch (state)
    {
    case 1001:
        decisionArea->enable(0);
        break;
    case 1002:
        Player* myself=dataInterface->getMyself();
        QList<Player*>players=dataInterface->getPlayerList();
        for(int i=0;i<players.size();i++)
        {
            if(myself->getID()==players[i]->getID())
                continue;
            if(players[i]->getCrossNum()>0)
                playerArea->enablePlayerItem(i);
        }
        break;
    }

}

void ShengQiang::onOkClicked()
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
    case 42:
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1'){
            HuiYaoAddition=false;
            emit sendCommand("1003;"+QString::number(myID)+";");            
            attackAction();
        }
        else if(text[0]=='2'){
            ChengJieAddition=false;
            emit sendCommand("1004;"+QString::number(myID)+";");           
            attackAction();
        }
        else if(text[0]=='3'){
            ShengGuangQiYuAddition=false;
            emit sendCommand("1007;"+QString::number(myID)+";");            
            attackAction();
        }
        break;
    case 1001:
        command="1001;";
        HuiYaoAddition=true;
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        command+=cardID+";"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
    case 1002:
        command="1002;";
        ChengJieAddition=true;
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=cardID+";"+targetID+";"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
    case 1005:
        command="1005;";
        command+=tipArea->getBoxCurrentText();
        emit sendCommand(command);
        gui->reset();
        break;
    case 1006:
        ShengGuangQiYuAddition=true;
        command="1006;";
        sourceID=QString::number(myID);
        command+=sourceID+";";
        emit sendCommand(command);
        gui->reset();;
        break;
    }
}

void ShengQiang::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
    case 1005:
        command="1005;0;";
        emit sendCommand(command);
        gui->reset();
        break;
    case 1001:
    case 1002:
    case 1006:
        if(actionFlag==0)
            normal();
        else if(actionFlag==1)
            attackAction();
        break;
    }
}
void ShengQiang::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==QStringLiteral("天枪"))
        TianQiang();
    else if(skill==QStringLiteral("地枪"))
        DiQiang();
}
void ShengQiang::additionalAction()
{
    Role::additionalAction();
    if(HuiYaoAddition)
        tipArea->addBoxItem(QStringLiteral("1.攻击行动(辉耀)"));
    if(ChengJieAddition)
        tipArea->addBoxItem(QStringLiteral("2.攻击行动(惩戒)"));
    if(ShengGuangQiYuAddition)
        tipArea->addBoxItem(QStringLiteral("3.攻击行动(圣光祈愈)"));

}

void ShengQiang::turnBegin()
{
    Role::turnBegin();
    HuiYaoAddition =false;
    ChengJieAddition=false;
    ShengGuangQiYuAddition=false;
}
