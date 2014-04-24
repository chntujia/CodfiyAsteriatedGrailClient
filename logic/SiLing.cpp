#include "SiLing.h"
enum CAUSE{
    BU_XIU = 1301,
    SHENG_DU = 1302,
    WEN_YI = 1303,
    SI_WANG_ZHI_CHU = 1304,
    MU_BEI_YUN_LUO = 1305
};

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
    SafeList<Card*> handcards=dataInterface->getHandCards();
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


void SiLing::WenYi()
{
    state = WEN_YI;
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
    state = SI_WANG_ZHI_CHU;
    Player* myself=dataInterface->getMyself();
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    handArea->setQuota(2,7);
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableAll();

    tipArea->setMsg(QStringLiteral("请选择移除的治疗，最少为2"));
    int cross = myself->getCrossNum();
    for(;cross>1;cross--)
        tipArea->addBoxItem(QString::number(cross));
    tipArea->showBox();
}

void SiLing::MuBeiYunLuo()
{
    state = MU_BEI_YUN_LUO;
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
    SafeList<Card*> selectedCards=handArea->getSelectedCards();
    try{
    switch (state)
    {
    case WEN_YI:
        decisionArea->enable(0);
        break;
    case SI_WANG_ZHI_CHU:
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

    }catch(int error){
        logic->onError(error);
    }
}

void SiLing::onOkClicked()
{
    Role::onOkClicked();
    SafeList<Card*> selectedCards;
    SafeList<Player*>selectedPlayers;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    try{
    switch(state)
    {
    case WEN_YI:
        action = newAction(ACTION_MAGIC_SKILL, WEN_YI);
        action->add_card_ids(selectedCards[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case SI_WANG_ZHI_CHU:
        action = newAction(ACTION_MAGIC_SKILL, SI_WANG_ZHI_CHU);
        action->add_dst_ids(selectedPlayers[0]->getID());
        action->add_args(tipArea->getBoxCurrentText().toInt());
        foreach(Card*ptr,selectedCards){
            action->add_card_ids(ptr->getID());
        }
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case MU_BEI_YUN_LUO:
        action = newAction(ACTION_MAGIC_SKILL, MU_BEI_YUN_LUO);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    }

    }catch(int error){
        logic->onError(error);
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
