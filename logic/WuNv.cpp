#include "WuNv.h"
enum CAUSE{
    XUE_ZHI_BEI_MING = 2301,
    TONG_SHENG_GONG_SI = 2302,
    XUE_ZHI_AI_SHANG = 2303,
    XUE_ZHI_AI_SHANG_ZHUAN_YI = 2331,
    NI_LIU = 2304,
    XUE_ZHI_ZU_ZHOU = 2351,
    XUE_ZHI_ZU_ZHOU_QI_PAI = 2352
};

WuNv::WuNv()
{
    makeConnection();
    setMyRole(this);
    tongShengID = -1;
    Button *tongShengGongSi, *niLiu, *xueZhiBeiMing, *xueZhiZuZhou;
    tongShengGongSi = new Button(3,QStringLiteral("同生共死"));
    buttonArea->addButton(tongShengGongSi);
    connect(tongShengGongSi,SIGNAL(buttonSelected(int)),this,SLOT(TongShengGongSi()));

    niLiu = new Button(4,QStringLiteral("逆流"));
    buttonArea->addButton(niLiu);
    connect(niLiu,SIGNAL(buttonSelected(int)),this,SLOT(NiLiu()));

    xueZhiBeiMing = new Button(5,QStringLiteral("血之悲鸣"));
    buttonArea->addButton(xueZhiBeiMing);
    connect(xueZhiBeiMing,SIGNAL(buttonSelected(int)),this,SLOT(XueZhiBeiMing()));

    xueZhiZuZhou = new Button(6,QStringLiteral("血之诅咒"));
    buttonArea->addButton(xueZhiZuZhou);
    connect(xueZhiZuZhou,SIGNAL(buttonSelected(int)),this,SLOT(XueZhiZuZhou()));
}

void WuNv::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    if(tongShengID==-1)
        buttonArea->enable(3);
    if(myself->getTap())
        buttonArea->enable(4);
    if(myself->getTap()&&handArea->checkSpecility(QStringLiteral("血之悲鸣")))
        buttonArea->enable(5);
    if(myself->getGem()>0)
        buttonArea->enable(6);
    unactionalCheck();
}

void WuNv::TongShengGongSi()
{
    state = TONG_SHENG_GONG_SI;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    playerArea->enableAll();

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void WuNv::XueZhiAiShang()
{
    state = XUE_ZHI_AI_SHANG;
    gui->reset();
    tipArea->addBoxItem(QStringLiteral("1.转移同生共死"));
    tipArea->addBoxItem(QStringLiteral("2.移除同生共死"));
    tipArea->showBox();
    tipArea->setMsg(QStringLiteral("请选择一项"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void WuNv::XueZhiAiShangZhuanYi()
{
    state = XUE_ZHI_AI_SHANG_ZHUAN_YI;
    gui->reset();

    playerArea->setQuota(1);
    playerArea->enableAll();
    if(tongShengID!=-1)
        playerArea->disablePlayerItem(tongShengID);

    decisionArea->disable(0);
    decisionArea->enable(1);
}

void WuNv::NiLiu()
{
    state = NI_LIU;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    handArea->setQuota(2);
    handArea->enableAll();

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void WuNv::XueZhiBeiMing()
{
    state = XUE_ZHI_BEI_MING;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    tipArea->setMsg(QStringLiteral("请选择【血之悲鸣】伤害"));
    for(int i =3;i>0;i--)
        tipArea->addBoxItem(QString::number(i));
    tipArea->showBox();

    handArea->setQuota(1);
    handArea->enableSpecility(QStringLiteral("血之悲鸣"));
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void WuNv::XueZhiZuZhou()
{
    state = XUE_ZHI_ZU_ZHOU;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->enableAll();

    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void WuNv::XueZhiZuZhouQiPai()
{
    state = XUE_ZHI_ZU_ZHOU_QI_PAI;
    gui->reset();
    tipArea->setMsg(QStringLiteral("请弃3张手牌（不足则全弃）"));
    SafeList<Card*> handcards=dataInterface->getHandCards();
    if(handcards.size()>=3)
        handArea->setQuota(3);
    else
        handArea->setQuota(handcards.size());
    handArea->enableAll();
}

void WuNv::cardAnalyse()
{
    Role::cardAnalyse();
    switch(state)
    {
    case TONG_SHENG_GONG_SI:
        playerArea->enableAll();
        break;
    case NI_LIU:
        decisionArea->enable(0);
        break;
    case XUE_ZHI_BEI_MING:
        playerArea->enableAll();
        break;
    case XUE_ZHI_ZU_ZHOU_QI_PAI:
        decisionArea->enable(0);
        break;
    }
}

void WuNv::onOkClicked()
{
    Role::onOkClicked();
    SafeList<Card*> selectedCards;
    SafeList<Player*>selectedPlayers;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;
    try{
    switch(state)
    {
    case TONG_SHENG_GONG_SI:
        action = newAction(ACTION_MAGIC_SKILL, TONG_SHENG_GONG_SI);
        action->add_dst_ids(selectedPlayers[0]->getID());
        tongShengID = selectedPlayers[0]->getID();
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case XUE_ZHI_AI_SHANG:
        if(tipArea->getBoxCurrentText()[0]=='2')
        {
			start = true;
            respond = newRespond(XUE_ZHI_AI_SHANG);
            respond->add_args(2);
            tongShengID = -1;
            emit sendCommand(network::MSG_RESPOND, respond);
            gui->reset();
        }
        else
        {
            gui->reset();
            XueZhiAiShangZhuanYi();
        }
        break;
    case XUE_ZHI_AI_SHANG_ZHUAN_YI:
		start = true;
        respond = newRespond(XUE_ZHI_AI_SHANG);
        respond->add_args(1);
        respond->add_dst_ids(selectedPlayers[0]->getID());
        tongShengID = selectedPlayers[0]->getID();
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
		break;
    case NI_LIU:
        action = newAction(ACTION_MAGIC_SKILL, NI_LIU);
        foreach(Card*ptr,selectedCards){
            action->add_card_ids(ptr->getID());
        }
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case XUE_ZHI_BEI_MING:
        action = newAction(ACTION_MAGIC_SKILL, XUE_ZHI_BEI_MING);
        action->add_dst_ids(selectedPlayers[0]->getID());
        action->add_card_ids(selectedCards[0]->getID());
        action->add_args(tipArea->getBoxCurrentText().toInt());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case XUE_ZHI_ZU_ZHOU:
        action = newAction(ACTION_MAGIC_SKILL, XUE_ZHI_ZU_ZHOU);
        action->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case XUE_ZHI_ZU_ZHOU_QI_PAI:
        respond = newRespond(XUE_ZHI_ZU_ZHOU_QI_PAI);
        foreach(Card*ptr,selectedCards)
        {
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

void WuNv::onCancelClicked()
{
    Role::onCancelClicked();

    network::Respond* respond;
    switch(state)
    {
    case TONG_SHENG_GONG_SI:
    case NI_LIU:
    case XUE_ZHI_BEI_MING:
    case XUE_ZHI_ZU_ZHOU:
        normal();
        break;
    case XUE_ZHI_AI_SHANG:
        respond = newRespond(XUE_ZHI_AI_SHANG);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case XUE_ZHI_AI_SHANG_ZHUAN_YI:
        respond = newRespond(XUE_ZHI_AI_SHANG);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
}

void WuNv::askForSkill(Command *cmd)
{
    switch(cmd->respond_id())
    {
    case XUE_ZHI_AI_SHANG:
        XueZhiAiShang();
        break;
    case XUE_ZHI_ZU_ZHOU_QI_PAI:
        XueZhiZuZhouQiPai();
        break;
    default:
        Role::askForSkill(cmd);
    }
}
