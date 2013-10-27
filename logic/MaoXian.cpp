#include "MaoXian.h"
MaoXian::MaoXian()
{
    makeConnection();
    setMyRole(this);
    connect(playerArea,SIGNAL(playerUnready()),this,SLOT(onUnready()));

    Button *qiZha,*touTianHuanRi,*teShuJiaGong;
    qiZha=new Button(3,QStringLiteral("欺诈"));
    buttonArea->addButton(qiZha);
    connect(qiZha,SIGNAL(buttonSelected(int)),this,SLOT(QiZha()));

    touTianHuanRi=new Button(4,QStringLiteral("偷天换日"));
    buttonArea->addButton(touTianHuanRi);
    connect(touTianHuanRi,SIGNAL(buttonSelected(int)),this,SLOT(TouTianHuanRi()));

    teShuJiaGong=new Button(5,QStringLiteral("特殊加工"));
    buttonArea->addButton(teShuJiaGong);
    connect(teShuJiaGong,SIGNAL(buttonSelected(int)),this,SLOT(TeShuJiaGong()));
}

void MaoXian::buy()
{
    Team*team=dataInterface->getMyTeam();

    int energy=team->getEnergy();

    state=4;
    decisionArea->enable(0);
    decisionArea->enable(1);
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    if(energy<4)
        tipArea->setMsg(QStringLiteral("你摸3张牌，你方战绩区加两宝石"));
    if(energy==4)
        tipArea->setMsg(QStringLiteral("战绩区星石已有4个，购买只增加一宝石"));
    if(energy==5)
        tipArea->setMsg(QStringLiteral("战绩区星石数目已达上限，购买将不再增加星石"));
}

void MaoXian::extract()
{
    state=1204;
    tipArea->reset();
    handArea->reset();
    playerArea->reset();
    decisionArea->disable(0);
    decisionArea->enable(1);
    playerArea->enableMate(true);
    playerArea->setQuota(1);
    tipArea->setMsg(QStringLiteral("请先选择要给予的对象"));
}

void MaoXian::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();

    if(dataInterface->getMyTeam()->getEnergy()>0)
        buttonArea->enable(2);
//欺诈
    QList<Card*>handcards=dataInterface->getHandCards();
    for(int i=0;i<handcards.size()-1;i++)
        for(int j=i+1;j<handcards.size();j++)
            if(handcards[i]->getElement()==handcards[j]->getElement()){
                buttonArea->enable(3);
                break;
            }

//偷天换日 特殊加工
    if(myself->getEnergy()>0){
        if(dataInterface->getOtherTeam()->getGem()>0 && !onceUsed2)
            buttonArea->enable(4);
        if(!onceUsed)
            buttonArea->enable(5);
    }
    unactionalCheck();
}

void MaoXian::attackOrMagic()
{
    Role::attackOrMagic();
    Player* myself=dataInterface->getMyself();
    QList<Card*>handcards=dataInterface->getHandCards();
    for(int i=0;i<handcards.size()-1;i++)
        for(int j=i+1;j<handcards.size();j++)
            if(handcards[i]->getElement()==handcards[j]->getElement()){
                buttonArea->enable(3);
                break;
            }
    if(myself->getEnergy()>0){
        if(dataInterface->getOtherTeam()->getGem()>0 && !onceUsed2)
            buttonArea->enable(4);
        if(!onceUsed)
            buttonArea->enable(5);
    }
}

void MaoXian::attackAction()
{
    Role::attackAction();
    QList<Card*>handcards=dataInterface->getHandCards();
    for(int i=0;i<handcards.size()-1;i++)
        for(int j=i+1;j<handcards.size();j++)
            if(handcards[i]->getElement()==handcards[j]->getElement()){
                buttonArea->enable(3);
                break;
            }
}

void MaoXian::QiZha()
{
    state=10;
    qizha=true;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(2,3);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableAll();
    tipArea->setMsg(QStringLiteral("请选择欺诈的属性（3张同系自动视为暗灭）："));
    tipArea->addBoxItem(QStringLiteral("1.风"));
    tipArea->addBoxItem(QStringLiteral("2.水"));
    tipArea->addBoxItem(QStringLiteral("3.火"));
    tipArea->addBoxItem(QStringLiteral("4.地"));
    tipArea->addBoxItem(QStringLiteral("5.雷"));
    tipArea->showBox();
}

void MaoXian::TeShuJiaGong()
{
    state=1202;
    Player*myself=dataInterface->getMyself();

    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->enable(0);

    tipArea->setMsg(QStringLiteral("请选择使用的能量："));
    if(myself->getCrystal()>=1)
        tipArea->addBoxItem(QStringLiteral("1.水晶"));
    if(myself->getGem()>=1)
        tipArea->addBoxItem(QStringLiteral("2.宝石"));

    tipArea->showBox();
}

void MaoXian::TouTianHuanRi()
{
    state=1203;
    Player*myself=dataInterface->getMyself();

    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->enable(0);

    tipArea->setMsg(QStringLiteral("请选择使用的能量："));
    if(myself->getCrystal()>=1)
        tipArea->addBoxItem(QStringLiteral("1.水晶"));
    if(myself->getGem()>=1)
        tipArea->addBoxItem(QStringLiteral("2.宝石"));

    tipArea->showBox();
}

void MaoXian::cardAnalyse()
{
    Role::cardAnalyse();
    QList<Card*> selectedCards=handArea->getSelectedCards();
    switch (state)
    {
    case 10:
        foreach(Card*ptr,selectedCards)
            if(ptr->getElement()!=selectedCards[0]->getElement()){
                playerArea->reset();
                decisionArea->disable(0);
                break;
            }            
        break;
    }
}

void MaoXian::playerAnalyse()
{
    Player*dst;
    int gem;
    int crystal;
    switch (state)
    {
    case 1204:
        dst=playerArea->getSelectedPlayers().at(0);
        gem=dataInterface->getMyTeam()->getGem();
        crystal=dataInterface->getMyTeam()->getCrystal();        
        tipArea->reset();
        tipArea->setMsg(QStringLiteral("请选择要提取的星石数："));
        switch(dst->getEnergyMax()-dst->getEnergy())
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
            decisionArea->enable(0);
            break;
        default:
            decisionArea->disable(0);
            break;
        }
        tipArea->showBox();        
        break;
    default:
        decisionArea->enable(0);
        break;
    }
}

void MaoXian::onOkClicked()
{
    if(qizha && state==10)
        state=1201;
    if(state==4)
        state=1205;
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    QString command;
    QString cardID;
    QString sourceID;
    QString targetID;
    QString text;
    int flag;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;

    switch(state)
    {
//额外行动询问
    case 42:
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1'){
            TeShuJiaGongAddition=false;
            respond = newRespond(1205);
            respond->add_args(1);
            emit sendCommand(network::MSG_RESPOND, respond);
            attackOrMagic();
        }
        else if(text[0]=='2'){
            TouTianHuanRiAddition=false;
            respond = newRespond(1206);
            respond->add_args(1);
            emit sendCommand(network::MSG_RESPOND, respond);
            attackOrMagic();
        }
        break;
//欺诈
    case 1201:
        action = newAction(1201);

        tipArea->addBoxItem(QStringLiteral("1.风"));
        tipArea->addBoxItem(QStringLiteral("2.水"));
        tipArea->addBoxItem(QStringLiteral("3.火"));
        tipArea->addBoxItem(QStringLiteral("4.地"));
        tipArea->addBoxItem(QStringLiteral("5.雷"));
        if(selectedCards.size()==3)
            flag=6;
        else
            flag=tipArea->getBoxCurrentIndex()+1;
        switch(flag)
        {
        case 1:
            respond->add_args(66);
            break;
        case 2:
            respond->add_args(133);
            break;
        case 3:
            respond->add_args(87);
            break;
        case 4:
            respond->add_args(45);
            break;
        case 5:
            respond->add_args(110);
            break;
        case 6:
            respond->add_args(39);
            break;
        }
        respond->add_dst_ids(selectedPlayers[0]->getID());
        foreach(Card*ptr,selectedCards){
            respond->add_args(ptr->getID());
            dataInterface->removeHandCard(ptr);
        }
        emit sendCommand(network::MSG_ACTION, action);
        qizha=false;
        gui->reset();
        break;
//特殊加工
    case 1202:
        action = newAction(1202);
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1')
            action->add_args(0);
        else
            action->add_args(1);
        onceUsed=true;
        TeShuJiaGongAddition=true;
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//偷天换日
    case 1203:
        action = newAction(1203);
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1')
            action->add_args(0);
        else
            action->add_args(1);
        TouTianHuanRiAddition=true;
        onceUsed2=true;
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//冒险家天堂
    case 1204:
        action = newAction(1204);
        action->add_dst_ids(selectedPlayers[0]->getID());
        text=tipArea->getBoxCurrentText();
        switch(text[0].digitValue())
        {
        case 1:
            action->add_args(2);
            action->add_args(0);
            break;
        case 2:
            action->add_args(0);
            action->add_args(2);
            break;
        case 3:
            action->add_args(1);
            action->add_args(1);
            break;
        case 4:
            action->add_args(1);
            action->add_args(0);
            break;
        case 5:
            action->add_args(0);
            action->add_args(1);
            break;
        }
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//购买
    case 1205:
//        action = newAction(network::ACTION_BUY);
        int stone=dataInterface->getMyTeam()->getEnergy();
        if(stone<4)
        {
            action->add_args(2);
            action->add_args(0);
        }
        else if(stone==4)
        {
            action->add_args(1);
            action->add_args(0);
        }
        else
        {
            action->add_args(0);
            action->add_args(0);
        }
        gui->reset();
        usedSpecial=true;
        usedAttack=usedMagic=false;
        emit sendCommand(network::MSG_ACTION, action);
        break;
    }
}

void MaoXian::onUnready()
{
    switch(state)
    {
    case 1204:
        tipArea->reset();
        normal();
        break;
    }
}

void MaoXian::onCancelClicked()
{
    Role::onCancelClicked();
    switch(state)
    {
//欺诈
    case 10:
    case 1201:
//特殊加工
    case 1202:
//偷天换日
    case 1203:
//冒险者天堂
    case 1204:
        if(actionFlag==4)
            attackOrMagic();
        if(actionFlag==0)
            normal();
        qizha=false;
        break;
    }
}

void MaoXian::additionalAction()
{
    Role::additionalAction();
    if(TeShuJiaGongAddition)
        tipArea->addBoxItem(QStringLiteral("1.攻击或法术行动（特殊加工）"));
    if(TouTianHuanRiAddition)
        tipArea->addBoxItem(QStringLiteral("2.攻击或法术行动（偷天换日）"));
}

void MaoXian::turnBegin()
{
    Role::turnBegin();
    TeShuJiaGongAddition=TouTianHuanRiAddition=false;
    onceUsed2=false;
    qizha=false;
}

