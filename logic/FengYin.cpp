#include "FengYin.h"

enum CAUSE{
    FENG_ZHI_FENG_YIN = 401,
    SHUI_ZHI_FENG_YIN = 402,
    HUO_ZHI_FENG_YIN = 403,
    DI_ZHI_FENG_YIN = 404,
    LEI_ZHI_FENG_YIN = 405,
    FA_SHU_JI_DONG = 406,
    WU_XI_SHU_FU = 407,
    FENG_YIN_PO_SUI = 408
};

FengYin::FengYin()
{
    makeConnection();
    setMyRole(this);
    connect(playerArea,SIGNAL(playerUnready()),this,SLOT(onUnready()));
//新建技能按钮
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

//用来检测当前可以发动的技能，几乎都是法术技能
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
    //反正都一样，就都叫风封印
    state = FENG_ZHI_FENG_YIN;
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
    state = WU_XI_SHU_FU;

    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->disable(0);
    playerArea->setQuota(1);
    playerArea->enableEnemy();
}

void FengYin::FengYinPoSui()
{
    state = FENG_YIN_PO_SUI;
    SafeList<Player*>players = dataInterface->getPlayerList();

    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->disable(0);
    playerArea->setQuota(1);

    for(int i=0;i<players.size();i++){
        if(players[i]->hasStatus()){
            playerArea->enablePlayerItem(i);
        }
    }
}

void FengYin::onUnready()
{
    switch(state)
    {
    case FENG_YIN_PO_SUI:
        tipArea->reset();
        FengYinPoSui();
        break;
    }
}

//当选的牌达到最低要求（quota）时，触发
void FengYin::cardAnalyse()
{
    Role::cardAnalyse();
    try{
    switch (state)
    {
    case FENG_ZHI_FENG_YIN:
        SafeList<Player*> players=dataInterface->getPlayerList();
        SafeList<Card*> selectedCards=handArea->getSelectedCards();
        playerArea->enableEnemy();
        //检测是否已有相同的封印
        for(int i=0;i<players.size();i++){
            for(int j=0;j<players[i]->getBasicStatus().size();j++){
                if(players[i]->checkBasicStatus(selectedCards[0]->getSpecialityList().at(0))){
                    playerArea->disablePlayerItem(i);
                }
            }
        }
        break;
    }
    }catch(int error){
        logic->onError(error);
    }
}

//一般不用覆写这个函数，基类的功能是当选的人达到最低要求（quota），enable确认键（0）
void FengYin::playerAnalyse()
{
    switch (state)
    {
    case FENG_YIN_PO_SUI:
        //展示基本状态区，一般用不到
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
    SafeList<Card*> selectedCards;
    SafeList<Player*>selectedPlayers;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    try{
    switch(state)
    {
//封印法术
    case FENG_ZHI_FENG_YIN:
        action = newAction(ACTION_MAGIC_SKILL, getMapping(selectedCards[0]->getElement()));
        action->add_card_ids(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//五系束缚
    case WU_XI_SHU_FU:
        action = newAction(ACTION_MAGIC_SKILL, WU_XI_SHU_FU);
        action->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//封印破碎
    case FENG_YIN_PO_SUI:
        action = newAction(ACTION_MAGIC_SKILL, FENG_YIN_PO_SUI);
        action->add_dst_ids(selectedPlayers[0]->getID());
        action->add_card_ids(tipArea->getSelectedCardID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    }
    }catch(int error){
        logic->onError(error);
    }
}

void FengYin::onCancelClicked()
{
    Role::onCancelClicked();
    switch(state)
    {
//封印法术
    case FENG_ZHI_FENG_YIN:
//五系束缚
    case WU_XI_SHU_FU:
//封印破碎
    case FENG_YIN_PO_SUI:
        normal();
        break;
    }
}

int FengYin::getMapping(QString element)
{
    if (element=="earth")
        return DI_ZHI_FENG_YIN;
    else if (element=="fire")
        return HUO_ZHI_FENG_YIN;
    else if (element=="water")
        return SHUI_ZHI_FENG_YIN;
    else if (element=="thunder")
        return LEI_ZHI_FENG_YIN;
    else if (element=="wind")
        return FENG_ZHI_FENG_YIN;
    else
        return 0;
}
