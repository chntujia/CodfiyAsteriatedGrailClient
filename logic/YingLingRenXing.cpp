#include "YingLingRenXing.h"

enum CAUSE{
    NU_HUO_MO_WEN = 2701,
	NU_HUO_YA_ZHI = 2702,
	ZHAN_WEN_SUI_JI = 2703,
	MO_WEN_RONG_HE = 2704,
	FU_WEN_GAI_ZAO = 2705,
	SHUANG_CHONG_HUI_XIANG =2706
};

YingLingRenXing::YingLingRenXing()
{
	makeConnection();
    setMyRole(this);
}

void YingLingRenXing::normal()
{
	Role::normal();
    unactionalCheck();
}

void YingLingRenXing::NuHuoMoWen()
{
	state = NU_HUO_MO_WEN;
	gui->reset();
    Player* myself=dataInterface->getMyself();
	int elements = handArea->getElementCount();
    tipArea->setMsg(QStringLiteral("请选择要发动的技能"));
    if(myself->getToken(0) > 0)
        tipArea->addBoxItem(QStringLiteral("1.怒火压制"));
    if(elements>1 && myself->getToken(1) > 0)
		tipArea->addBoxItem(QStringLiteral("2.魔纹融合"));
	tipArea->showBox();
	decisionArea->enable(0);
	decisionArea->enable(1);
}

void YingLingRenXing::MoWenRongHe()
{
	state = MO_WEN_RONG_HE;
	gui->reset();
	Player* myself=dataInterface->getMyself();
    tipArea->setMsg(QStringLiteral("弃异系牌发动【魔纹融合】"));
	if(myself->getTap()) {
        tipArea->setMsg(QStringLiteral("弃异系牌发动【魔纹融合】，并选择额外翻转魔纹数量"));
		for(int i =0; i < myself->getToken(1);i ++)
			tipArea->addBoxItem(QString::number(i));
		tipArea->showBox();
	}
	handArea->enableAll();
	handArea->setQuota(2,9);

	decisionArea->enable(1);
	decisionArea->disable(0);
}

void YingLingRenXing::ZhanWenSuiJi()
{
	state = ZHAN_WEN_SUI_JI;
	gui->reset();
	Player* myself=dataInterface->getMyself();
    tipArea->setMsg(QStringLiteral("弃同系牌发动【战纹碎击】"));
	if(myself->getTap()) {
        tipArea->setMsg(QStringLiteral("弃同系牌发动【战纹碎击】，并选择额外翻转战纹数量"));
		for(int i =0; i < myself->getToken(0);i ++)
			tipArea->addBoxItem(QString::number(i));
		tipArea->showBox();
	}
	handArea->enableAll();
	handArea->setQuota(2,9);

	decisionArea->enable(1);
	decisionArea->disable(0);
}

void YingLingRenXing::FuWenGaiZao()
{
	state = FU_WEN_GAI_ZAO;
	gui->reset();
    tipArea->setMsg(QStringLiteral("发动【符文改造】，并调整战纹数量"));
	for(int i =0; i < 4;i ++)
		tipArea->addBoxItem(QString::number(i));
	tipArea->showBox();
	decisionArea->enable(0);
	decisionArea->enable(1);
}

void YingLingRenXing::ShuangChongHuiXiang(int previous)
{
	state = SHUANG_CHONG_HUI_XIANG;
	gui->reset();
	tipArea->setMsg(QStringLiteral("发动【双重回响】，并选择目标角色"));
	playerArea->enableAll();
    playerArea->disablePlayerItem(previous);
	playerArea->setQuota(1);

	decisionArea->enable(1);
	decisionArea->disable(0);
}

void YingLingRenXing::cardAnalyse()
{
	Role::cardAnalyse();
    QList<Card*> selectedCards=handArea->getSelectedCards();
    bool cardReady = true;
    switch(state)
    {
    case MO_WEN_RONG_HE:
    	if(selectedCards.size()<2)
            decisionArea->disable(0);
        else {
        	foreach(Card*ptr,selectedCards)
                for(int i=0;i<selectedCards.size();i++){
                    if(ptr->getElement()==selectedCards[i]->getElement() && ptr->getID()!=selectedCards[i]->getID()){
                        decisionArea->disable(0);
                        cardReady=false;
                        break;
                    }
                }
            if(cardReady)
                decisionArea->enable(0);
        }
        break;
    case ZHAN_WEN_SUI_JI:
    	if(selectedCards.size()<2)
            decisionArea->disable(0);
        else
        {
            foreach(Card*ptr,selectedCards)
                for(int i=0;i<selectedCards.size();i++){
                    if(ptr->getElement()!=selectedCards[i]->getElement()){
                        playerArea->reset();
                        decisionArea->disable(0);
                        cardReady=false;
                        break;
                    }
                }
            if(cardReady)
                decisionArea->enable(0);
        }
    	break;
    }
}

void YingLingRenXing::onOkClicked()
{
	Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;
    Player* myself=dataInterface->getMyself();


    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Respond* respond;

    int choice;
    switch(state)
    {
    case NU_HUO_MO_WEN:
	    choice = tipArea->getBoxCurrentText()[0].digitValue();
	    if(choice == 1) {
            respond = newRespond(NU_HUO_MO_WEN);
	    	respond->add_args(1);
	    	emit sendCommand(network::MSG_RESPOND, respond);
            gui->reset();
	    }
	    else {
	    	gui->reset();
	    	MoWenRongHe();
	    }
	    break;
	case MO_WEN_RONG_HE:
        respond = newRespond(NU_HUO_MO_WEN);
        respond->add_args(2);
        if(myself->getTap())
            choice = tipArea->getBoxCurrentText()[0].digitValue();
        else
            choice = 0;
        respond->add_args(choice);
        foreach(Card*ptr,selectedCards){
            respond->add_card_ids(ptr->getID());
        }
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case ZHAN_WEN_SUI_JI:
    	respond = newRespond(ZHAN_WEN_SUI_JI);
        respond->add_args(1);
    	if(myself->getTap())
            choice = tipArea->getBoxCurrentText()[0].digitValue();
        else
            choice = 0;
        respond->add_args(choice);
        foreach(Card*ptr,selectedCards){
            respond->add_card_ids(ptr->getID());
        }
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case FU_WEN_GAI_ZAO:
        respond = newRespond(FU_WEN_GAI_ZAO);
        choice = tipArea->getBoxCurrentText()[0].digitValue();
        respond->add_args(1);
        respond->add_args(choice);
        emit sendCommand(network::MSG_RESPOND, respond);
        start = true;
        gui->reset();
        break;
    case SHUANG_CHONG_HUI_XIANG:
    	respond = newRespond(SHUANG_CHONG_HUI_XIANG);
    	respond->add_args(1);
        respond->add_dst_ids(selectedPlayers[0]->getID());
    	emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
}

void YingLingRenXing::onCancelClicked()
{
	Role::onCancelClicked();
	network::Respond* respond;
	switch(state)
    {
    case NU_HUO_MO_WEN:
    case ZHAN_WEN_SUI_JI:
    case FU_WEN_GAI_ZAO:
    case SHUANG_CHONG_HUI_XIANG:
    	respond = newRespond(state);
        respond->add_args(0);
    	emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case MO_WEN_RONG_HE:
    	gui->reset();
    	NuHuoMoWen();
    	break;
    }
}

void YingLingRenXing::askForSkill(network::Command* cmd)
{
    gui->alert();
	switch(cmd->respond_id())
	{
	case NU_HUO_MO_WEN:
		NuHuoMoWen();
		break;
	case ZHAN_WEN_SUI_JI:
		ZhanWenSuiJi();
		break;
	case FU_WEN_GAI_ZAO:
		FuWenGaiZao();
		break;
	case SHUANG_CHONG_HUI_XIANG:
        ShuangChongHuiXiang(cmd->args(0));
		break;
	default:
		Role::askForSkill(cmd);
	}
}
