#include "GeDouJia.h"
enum CAUSE{
    NIAN_QI_LI_CHANG = 2001,
    XU_LI_YI_JI = 2002,
    NIAN_DAN = 2003,
    NIAN_DAN_SELF = 2031,
    BAI_SHI_HUAN_LONG_QUAN = 2004,
    CANG_YAN_ZHI_HUN = 2005,
    DOU_SHEN_TIAN_QU = 2006,
    XU_LI_CANG_YAN = 2025,
    BAI_SHI_DOU_SHEN = 2046
};

GeDouJia::GeDouJia()
{
    makeConnection();
    setMyRole(this);
}

void GeDouJia::XuLiCangYan(int skillID)
{
    gui->reset();
    Player* myself=dataInterface->getMyself();
    state=XU_LI_CANG_YAN;
    decisionArea->enable(0);
    decisionArea->enable(1);
    if(myself->getTap()==0)
        baiShiUsed=false;
    tipArea->setMsg(QStringLiteral("请选择要发动的技能："));
    bool xuli = false, cangyan = false;
    if(skillID == XU_LI_YI_JI)
        xuli = true;
    if(skillID == CANG_YAN_ZHI_HUN)
        cangyan = true;
    if(skillID == XU_LI_CANG_YAN)
        xuli = cangyan = true;
    if(xuli && !baiShiUsed)
        tipArea->addBoxItem(QStringLiteral("1.蓄力一击"));
    if(xuli && baiShiUsed)
        tipArea->addBoxItem(QStringLiteral("1.蓄力一击，且退出【百式幻龙拳】"));
    if(cangyan)
        tipArea->addBoxItem(QStringLiteral("2.苍炎之魂"));
    tipArea->showBox();
}

void GeDouJia::NianDan()
{
    state=NIAN_DAN;
    gui->reset();
    tipArea->setMsg(QStringLiteral("选择念弹目标或取消"));
    playerArea->enableEnemy();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void GeDouJia::BaiShiDouShen()
{
    state = BAI_SHI_DOU_SHEN;
    gui->reset();
    tipArea->setMsg(QStringLiteral("请选择要发动的技能"));
    tipArea->addBoxItem(QStringLiteral("1.百式幻龙拳"));
    tipArea->addBoxItem(QStringLiteral("2.斗神天驱"));
    tipArea->showBox();
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void GeDouJia::BaiShiHuanLongQuan()
{
    state=BAI_SHI_HUAN_LONG_QUAN;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动百式幻龙拳？"));
    SafeList<Card*> handcards=dataInterface->getHandCards();
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
    }
    if(flag)
        decisionArea->enable(0);
}

void GeDouJia::DouShenTianQu()
{
    state=DOU_SHEN_TIAN_QU;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动斗神天驱？如是，请选择要弃的牌"));
    SafeList<Card*> handcards=dataInterface->getHandCards();
    cardReady=false;
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
    }
    if(flag)
    {

        if(n<4)
            decisionArea->enable(0);
        else{
            handArea->enableAll();
            handArea->setQuota(n-3);
            cardReady=true;
            }
    }
}

void GeDouJia::cardAnalyse()
{
    tipArea->reset();
    Role::cardAnalyse();

    switch(state)
    {
    case DOU_SHEN_TIAN_QU:
        if(cardReady)
            decisionArea->enable(0);
        break;
    }
}

void GeDouJia::playerAnalyse()
{
    Role::playerAnalyse();
    Player*myself = dataInterface->getMyself();
    SafeList<Card*> selectedCards=handArea->getSelectedCards();
    SafeList<Player*> selectedPlayers=playerArea->getSelectedPlayers();
    if(myself->getTap()==0){
        baiShiUsed=false;
        baiShiTargetID=-1;
    }
    try{
    switch(state)
    {
    case 1:
    case 10:
    case 12:
        if(baiShiUsed)
        {
            tipArea->reset();
            if(selectedCards[0]->getType()=="magic")
            {
                tipArea->setMsg(QStringLiteral("这会退出【百式幻龙拳】，是否确定？"));
            }
            else if(baiShiFirst)
            {
                baiShiTargetID=selectedPlayers[0]->getID();
            }
            else if(selectedPlayers[0]->getID()!=baiShiTargetID)
            {
                tipArea->setMsg(QStringLiteral("这会退出【百式幻龙拳】，是否确定？"));
            }
            decisionArea->enable(0);
        }
        break;
    }
    }catch(int error){
        logic->onError(error);
    }
}

void GeDouJia::buy()
{
    Player*myself = dataInterface->getMyself();
    if(myself->getTap()==0){
        baiShiUsed=false;
        baiShiTargetID=-1;
    }
    if(!baiShiUsed)
        Role::buy();
    else{
        Team*team=dataInterface->getMyTeam();

        int energy=team->getEnergy();

        state=4;
        decisionArea->enable(0);
        decisionArea->enable(1);
        handArea->reset();
        playerArea->reset();
        tipArea->reset();

        if(energy<4)
            tipArea->setMsg(QStringLiteral("你将会退出【百式幻龙拳】,执行【购买】"));
        if(energy==4)
        {
            tipArea->setMsg(QStringLiteral("你将会退出【百式幻龙拳】,执行【购买】"));
            tipArea->addBoxItem(QStringLiteral("1.一个宝石"));
            tipArea->addBoxItem(QStringLiteral("2.一个水晶"));
            tipArea->showBox();
        }
        if(energy==5)
        {
            tipArea->setMsg(QStringLiteral("你将会退出【百式幻龙拳】,执行【购买】"));
        }
    }
}

void GeDouJia::synthetize()
{
    Player*myself = dataInterface->getMyself();
    if(myself->getTap()==0){
        baiShiUsed=false;
        baiShiTargetID=-1;
    }
    if(!baiShiUsed)
        Role::synthetize();
    else{
        int gem,crystal;
        Team* team;

        team=dataInterface->getMyTeam();
        gem=team->getGem();
        crystal=team->getCrystal();

        state=5;
        decisionArea->enable(0);
        decisionArea->enable(1);
        tipArea->reset();
        handArea->reset();
        playerArea->reset();

        tipArea->setMsg(QStringLiteral("你将会退出【百式幻龙拳】,执行【合成】"));
        if(crystal>=3)
            tipArea->addBoxItem(QStringLiteral("1.三个水晶"));
        if(crystal>=2&&gem>=1)
            tipArea->addBoxItem(QStringLiteral("2.两个水晶和一个宝石"));
        if(crystal>=1&&gem>=2)
            tipArea->addBoxItem(QStringLiteral("3.一个水晶和两个宝石"));
        if(gem>=3)
            tipArea->addBoxItem(QStringLiteral("4.三个宝石"));
        tipArea->showBox();
    }
}

void GeDouJia::extract()
{
    Player*myself = dataInterface->getMyself();
    if(myself->getTap()==0){
        baiShiUsed=false;
        baiShiTargetID=-1;
    }
    if(!baiShiUsed)
        Role::extract();
    else{
        int gem,crystal;
        Team* team;
        Player* myself=dataInterface->getPlayerList().at(myID);

        team=dataInterface->getMyTeam();
        gem=team->getGem();
        crystal=team->getCrystal();

        state=6;
        decisionArea->enable(0);
        decisionArea->enable(1);
        tipArea->reset();
        handArea->reset();
        playerArea->reset();

        tipArea->setMsg(QStringLiteral("你将会退出【百式幻龙拳】,执行【提炼】"));
        switch(myself->getEnergyMax()-myself->getEnergy())
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
        }
        tipArea->showBox();
    }
}

void GeDouJia::onOkClicked()
{
    Role::onOkClicked();
    SafeList<Player*>selectedPlayers;
    SafeList<Card*> selectedCards;

    QString text;
    selectedPlayers=playerArea->getSelectedPlayers();
    selectedCards=handArea->getSelectedCards();

    network::Respond* respond;
    try{
    switch(state)
    {
    case 1:
    case 10:
    case 12:
        if(baiShiUsed)
            baiShiFirst=false;
        break;
    case XU_LI_CANG_YAN:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(XU_LI_CANG_YAN);

        text=tipArea->getBoxCurrentText();
        if(text[0]=='1')
            respond->add_args(1);
        else
            respond->add_args(2);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case NIAN_DAN:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(NIAN_DAN);
        respond->add_args(1);
        respond->add_dst_ids(selectedPlayers[0]->getID());

        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case BAI_SHI_DOU_SHEN:

        text = tipArea->getBoxCurrentText();
        if(text[0]=='1')
            BaiShiHuanLongQuan();
        else
            DouShenTianQu();
        break;
    case BAI_SHI_HUAN_LONG_QUAN:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(BAI_SHI_DOU_SHEN);
        respond->add_args(1);

        baiShiUsed=true;
        baiShiFirst=true;
        start=true;
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case DOU_SHEN_TIAN_QU:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(BAI_SHI_DOU_SHEN);
        respond->add_args(2);

        start=true;
        respond->add_args(selectedCards.size());
        foreach(Card*ptr,selectedCards){
            respond->add_card_ids(ptr->getID());
        }
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
    }catch(int error){
        logic->onError(error);
    }
}

void GeDouJia::onCancelClicked()
{
    Role::onCancelClicked();

    network::Respond* respond;

    switch(state)
    {
    case XU_LI_CANG_YAN:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(XU_LI_CANG_YAN);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case NIAN_DAN:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(NIAN_DAN);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case BAI_SHI_DOU_SHEN:
    case BAI_SHI_HUAN_LONG_QUAN:
    case DOU_SHEN_TIAN_QU:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(BAI_SHI_DOU_SHEN);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
}

void GeDouJia::askForSkill(Command* cmd)
{
    switch(cmd->respond_id())
    {
    case XU_LI_YI_JI:
    case CANG_YAN_ZHI_HUN:
    case XU_LI_CANG_YAN:
        XuLiCangYan(cmd->respond_id());
        break;
    case NIAN_DAN:
        NianDan();
        break;
    case BAI_SHI_HUAN_LONG_QUAN:
        BaiShiHuanLongQuan();
        break;
    case DOU_SHEN_TIAN_QU:
        DouShenTianQu();
        break;
    case BAI_SHI_DOU_SHEN:
        BaiShiDouShen();
        break;
    default:
        Role::askForSkill(cmd);
    }
}

