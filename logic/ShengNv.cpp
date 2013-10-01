#include "ShengNv.h"

ShengNv::ShengNv()
{
    makeConnection();
setMyRole(this);
    Button *zhiLiaoShu, *zhiYuZhiGuang, *shengLiao;
    zhiLiaoShu = new Button(3,QStringLiteral("治疗术"));
    buttonArea->addButton(zhiLiaoShu);
    connect(zhiLiaoShu,SIGNAL(buttonSelected(int)),this,SLOT(ZhiLiaoShu()));

    zhiYuZhiGuang=new Button(4, QStringLiteral("治愈之光"));
    buttonArea->addButton(zhiYuZhiGuang);
    connect(zhiYuZhiGuang,SIGNAL(buttonSelected(int)),this,SLOT(ZhiYuZhiGuang()));

    shengLiao=new Button(5,QStringLiteral("圣疗"));
    buttonArea->addButton(shengLiao);
    connect(shengLiao,SIGNAL(buttonSelected(int)),this,SLOT(ShengLiao()));
}

void ShengNv::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();

    if (handArea->checkSpecility(QStringLiteral("治疗术")))
        buttonArea->enable(3);
    if (handArea->checkSpecility(QStringLiteral("治愈之光")))
        buttonArea->enable(4);
    //圣疗
    if (myself->getEnergy()>0 && !onceUsed)
        buttonArea->enable(5);
    unactionalCheck();
}


void ShengNv::attackOrMagic()
{
    Role::attackOrMagic();
    Player* myself=dataInterface->getMyself();
    if (handArea->checkSpecility(QStringLiteral("治疗术")))
        buttonArea->enable(3);
    if (handArea->checkSpecility(QStringLiteral("治愈之光")))
        buttonArea->enable(4);
    if (myself->getEnergy()>0 && !onceUsed)
        buttonArea->enable(5);
}


void ShengNv::ZhiLiaoShu()
{
    state=602;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableSpecility(QStringLiteral("治疗术"));
}

void ShengNv::ZhiYuZhiGuang()
{
    state=603;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();


    handArea->setQuota(1);
    playerArea->setQuota(1,3);
    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableSpecility(QStringLiteral("治愈之光"));
}

void ShengNv::LianMin()
{
    state=604;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动怜悯？"));
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
            else if(handcards[i]->getProperty()==QStringLiteral("圣"))
            {
                flag=true;
                break;
            }
    }
    if(flag||dataInterface->getMyself()->getEnergy()>1)
        decisionArea->enable(0);
}
void ShengNv::ShengLiao()
{
    int gem, crystal;
    Player*myself=dataInterface->getMyself();
    gem=myself->getGem();
    crystal=myself->getCrystal();

    state=651;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->disable(0);
    playerArea->setQuota(1);
    playerArea->enableAll();

    tipArea->setMsg(QStringLiteral("每次点选一人分配一点治疗，共三次"));
    if(crystal>=1)
        tipArea->addBoxItem(QStringLiteral("1.水晶"));
    if(gem>=1)
        tipArea->addBoxItem(QStringLiteral("2.宝石"));

    tipArea->showBox();
}
void ShengNv::ShengLiao2()
{
    state=652;
    onceUsed=true;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    buttonArea->reset();

    tipArea->setMsg(QStringLiteral("请选择分配第二点治疗"));
    playerArea->enableAll();
    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->disable(1);
    decisionArea->disable(0);

}
void ShengNv::ShengLiao3()
{
    state=653;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    buttonArea->reset();

    tipArea->setMsg(QStringLiteral("请选择分配第三点治疗"));
    playerArea->enableAll();
    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->disable(1);
    decisionArea->disable(0);

}
void ShengNv::BingShuangDaoYan()
{
    state=601;

    tipArea->setMsg(QStringLiteral("请选择冰霜祷言的目标"));
    playerArea->enableAll();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void ShengNv::cardAnalyse()
{
    Role::cardAnalyse();
    switch (state)
    {
    case 602:
        playerArea->enableAll();
        break;
    case 603:
        playerArea->enableAll();
        break;
    }
}

void ShengNv::onOkClicked()
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
//额外行动询问
    case 42:
        text=tipArea->getBoxCurrentText();
        //攻击或法术行动
        if(text[0]=='1'){
            ShengLiaoAddition=false;
            emit sendCommand("606;"+QString::number(myID)+";");
            attackOrMagic();
        }
        break;
//冰霜祷言
    case 601:
        command="601;";
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";";
        emit sendCommand(command);
        gui->reset();
        break;
//治疗术
    case 602:
        command="602;";
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=cardID+";"+targetID+";"+sourceID+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
//治愈之光
    case 603:
        command="603;";
        int n;
        n=selectedPlayers.size();
        cardID=QString::number(selectedCards[0]->getID());
        sourceID=QString::number(myID);
        command+=cardID+";"+QString::number(myID)+";"+QString::number(n)+";";
        for(int i=0; i<n; i++)
            command+=QString::number(selectedPlayers[i]->getID())+";";
        dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(command);
        gui->reset();
        break;
//怜悯
    case 604:
        start=true;
        command="604;1;";
        emit sendCommand(command);
        gui->reset();
        break;
//圣疗
    case 651:        
        command="605;";
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1')
            command+="0;";
        else
            command+="1;";
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";";
        ShengLiao2();
        break;
    case 652:
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";";
        ShengLiao3();
        break;
    case 653:
        targetID=QString::number(selectedPlayers[0]->getID());
        command+=targetID+";";
        sourceID=QString::number(myID);
        command+=sourceID+";";
        emit sendCommand(command);
        ShengLiaoAddition=true;
        gui->reset();
        break;
    }
}


void ShengNv::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;
    switch(state)
    {
//怜悯
    case 604:
        command="604;0;";
        emit sendCommand(command);
        gui->reset();
        break;
//冰霜祷言
//治疗术
    case 602:
//治愈之光
    case 603:
//圣疗
    case 651:
        if(actionFlag==0)
            normal();
        else if(actionFlag==4)
            attackOrMagic();
        break;
    }
}

void ShengNv::askForSkill(QString skill)
{
    Role::askForSkill(skill);
    if(skill==QStringLiteral("冰霜祷言"))
        BingShuangDaoYan();
    else if(skill==QStringLiteral("怜悯"))
        LianMin();
}

void ShengNv::additionalAction()
{
    Role::additionalAction();
    if(ShengLiaoAddition)
        tipArea->addBoxItem(QStringLiteral("1.攻击或法术行动（圣疗）"));
}

void ShengNv::turnBegin()
{
    Role::turnBegin();
    ShengLiaoAddition = false;
}
