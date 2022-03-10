#include "YueNv.h"

enum CAUSE{
	XIN_YUE_BI_HU = 3501,
	AN_YUE_ZU_ZHOU = 3502,
	MEI_DU_SHA = 3503,
	MEI_DU_SHA_2 = 35031,
	YUE_ZHI_LUN_HUI = 3504,
	YUE_ZHI_LUN_HUI_2 = 35041,
	YUE_DU = 3505,
	AN_YUE_ZHAN = 3506,
	CANG_BAI_ZHI_YUE = 3507,
	CANG_BAI_ZHI_YUE_2 = 35071
};

YueNv::YueNv()
{
    makeConnection();
    setMyRole(this);

	Button *checkCover, *cangBai;
	checkCover = new Button(10, QStringLiteral("查看暗月"));
	buttonArea->addOutsideTurnButton(checkCover);
	checkCover->setVisible(true);
	checkCover->setEnabled(true);
	connect(checkCover, SIGNAL(buttonSelected(int)), gui, SLOT(showCoverArea()));
	connect(checkCover, SIGNAL(buttonUnselected(int)), gui, SLOT(closeCoverArea()));

    cangBai = new Button(3,QStringLiteral("苍白之月"));
    buttonArea->addButton(cangBai);
    connect(cangBai,SIGNAL(buttonSelected(int)),this,SLOT(CangBaiZhiYue()));
}

void YueNv::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    SafeList<Card*> handcards=dataInterface->getHandCards();
	if(myself->getGem() > 0)
		buttonArea->enable(3);
    unactionalCheck();
}

void YueNv::XinYueBiHu()
{
    state =XIN_YUE_BI_HU;
	gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动新月庇护？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void YueNv::MeiDuSha(int cardId)
{
	meidusha_card = cardId;
    state = MEI_DU_SHA;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动美杜莎之眼？"));
	gui->showCoverArea(true);
	HandArea *coverArea = gui->getCoverArea();
	coverArea->enableElement(dataInterface->getCard(cardId)->getElement());
	coverArea->setQuota(1);

	decisionArea->enable(1);
    decisionArea->disable(0);
}

void YueNv::MeiDuSha_2()
{
	state = MEI_DU_SHA_2;
	gui->reset();
	tipArea->setMsg(QStringLiteral("选择了法术牌，请选择法伤目标"));
	playerArea->enableEnemy();
	playerArea->setQuota(1);
	decisionArea->enable(1);
	decisionArea->disable(0);
}

void YueNv::YueZhiLunHui()
{
    state = YUE_ZHI_LUN_HUI;
	gui->reset();
    tipArea->setMsg(QStringLiteral("月之轮回：选择要发动的效果"));

	if (dataInterface->getCoverCards().size() > 0)
	{
		tipArea->addBoxItem(QStringLiteral("1.移除1暗月，目标+1治疗"));

	}
	Player* myself = dataInterface->getMyself();
	if (myself->getCrossNum() > 0)
	{
		tipArea->addBoxItem(QStringLiteral("2.移除1治疗，你+1新月"));
	}
	
	tipArea->showBox();
    decisionArea->enable(1);
    decisionArea->enable(0);
}

void YueNv::YueZhiLunHui_2()
{
	state = YUE_ZHI_LUN_HUI_2;
	gui->reset();
	tipArea->setMsg(QStringLiteral("月之轮回：选择移除的暗月 和 治疗目标"));
	gui->showCoverArea(true);
	HandArea *coverArea = gui->getCoverArea();
	coverArea->enableAll();
	coverArea->setQuota(1);
	decisionArea->enable(1);
	decisionArea->disable(0);
}

void YueNv::YueDu()
{
	state = YUE_DU;
	gui->reset();
	tipArea->setMsg(QStringLiteral("若发动月渎，请选择目标对手"));
	playerArea->enableEnemy();
	playerArea->setQuota(1);
	decisionArea->enable(1);
	decisionArea->disable(0);
}

void YueNv::AnYueZhan()
{
	state = AN_YUE_ZHAN;
	gui->reset();
	tipArea->setMsg(QStringLiteral("若发动暗月斩，选择要移除的暗月"));
	gui->showCoverArea(true);
	HandArea *coverArea = gui->getCoverArea();
	coverArea->enableAll();
	coverArea->setQuota(1,2);

	decisionArea->enable(1);
	decisionArea->disable(0);
}
	
void YueNv::CangBaiZhiYue()
{
	state = CANG_BAI_ZHI_YUE;
	gui->reset();
	Player* myself = dataInterface->getMyself();
	tipArea->setMsg(QStringLiteral("苍白之月：选择要发动的效果"));
	if(myself->getToken(1) > 2)
		tipArea->addBoxItem(QStringLiteral("1.移除3石化，你+1攻击行动，此攻击无法应战。并获得1额外回合"));
	tipArea->addBoxItem(QStringLiteral("2.移除X【新月】，你+1【石化】，弃1张牌，对目标对手造成（X+1）点法术伤害③"));
	
	tipArea->showBox();

	decisionArea->enable(1);
	decisionArea->enable(0);
}

void YueNv::CangBaiZhiYue_2()
{
	state = CANG_BAI_ZHI_YUE_2;
	gui->reset();
	tipArea->setMsg(QStringLiteral("苍白之月：选择法伤目标和移除新月的数量"));
	Player* myself = dataInterface->getMyself();
	int num = myself->getToken(0);
	for (; num > -1; num--)
	{
		tipArea->addBoxItem(QString::number(num));
	}
	tipArea->showBox();
	
	playerArea->enableAll();
	playerArea->setQuota(1);

	decisionArea->enable(1);
	decisionArea->disable(0);
}

void YueNv::cardAnalyse()
{
    Role::cardAnalyse();
	SafeList<Card*> selectedCards = handArea->getSelectedCards();
    switch (state)
    {
    case MEI_DU_SHA:
		decisionArea->enable(0);
        break;
    }
}

void YueNv::coverCardAnalyse()
{
	Role::coverCardAnalyse();
	SafeList<Card*> selectedCoverCards = this->coverArea->getSelectedCards();
	try {
		switch (state)
		{
		case MEI_DU_SHA:
			decisionArea->enable(0);
			break;
		case AN_YUE_ZHAN:
			decisionArea->enable(0);
			break;
		case YUE_ZHI_LUN_HUI_2:
			playerArea->enableAll();
			playerArea->setQuota(1);
			break;
		}
	}
	catch (int error) {
		logic->onError(error);
	}
}
void YueNv::askForSkill(network::Command* cmd)
{
	//Role::askForSkill(skill);
	switch (cmd->respond_id())
	{
	case XIN_YUE_BI_HU:
		XinYueBiHu();
		break;
	case MEI_DU_SHA:
		MeiDuSha(cmd->args(0));
		break;
	case YUE_ZHI_LUN_HUI:
		YueZhiLunHui();
		break;
	case YUE_DU:
		YueDu();
		break;
	case AN_YUE_ZHAN:
		AnYueZhan();
		break;
	default:
		Role::askForSkill(cmd);
	}
}

void YueNv::onOkClicked()
{
    Role::onOkClicked();
    SafeList<Card*> selectedCards = handArea->getSelectedCards();
	SafeList<Card*> selectedCoverCards = coverArea->getSelectedCards();
    SafeList<Player*>selectedPlayers = playerArea->getSelectedPlayers();
	int choice = tipArea->getBoxCurrentText()[0].digitValue();
    network::Action* action;
    network::Respond* respond;
    try{
    switch(state)
    {
    case XIN_YUE_BI_HU:
		respond = new network::Respond();
		respond->set_src_id(myID);
		respond->add_args(1);
		respond->set_respond_id(XIN_YUE_BI_HU);
		emit sendCommand(network::MSG_RESPOND, respond);
		gui->reset();
		break;
    case MEI_DU_SHA:
		if (selectedCoverCards[0]->getType() == "magic")
		{	
			meidusha_anyue = selectedCoverCards[0]->getID();
			MeiDuSha_2();
			
		}
		else
		{
			respond = new network::Respond();
			respond->set_src_id(myID);
			respond->add_args(1);
			respond->add_card_ids(selectedCoverCards[0]->getID());
			respond->set_respond_id(MEI_DU_SHA);
			emit sendCommand(network::MSG_RESPOND, respond);
			gui->reset();
		}
        break;
	case MEI_DU_SHA_2:
			respond = new network::Respond();
			respond->set_src_id(myID);
			respond->add_args(2);
			respond->add_card_ids(meidusha_anyue);
			respond->add_dst_ids( selectedPlayers[0]->getID() );
			respond->set_respond_id(MEI_DU_SHA);
			emit sendCommand(network::MSG_RESPOND, respond);
			gui->reset();
		break;
    case YUE_ZHI_LUN_HUI:
		if (choice == 1)
			YueZhiLunHui_2();
		else
		{
			respond = new network::Respond();
			respond->set_src_id(myID);
			respond->add_args(2);
			respond->set_respond_id(YUE_ZHI_LUN_HUI);
			emit sendCommand(network::MSG_RESPOND, respond);
			gui->reset();
		}
        break;
	case YUE_ZHI_LUN_HUI_2:
			respond = new network::Respond();
			respond->set_src_id(myID);
			respond->add_args(1);
			respond->add_card_ids( selectedCoverCards[0]->getID() );
			respond->add_dst_ids( selectedPlayers[0]->getID() );
			respond->set_respond_id(YUE_ZHI_LUN_HUI);
			emit sendCommand(network::MSG_RESPOND, respond);
			gui->reset();
		break;
	case YUE_DU:
		respond = new network::Respond();
		respond->set_src_id(myID);
		respond->add_args(1);
		respond->add_dst_ids(selectedPlayers[0]->getID());
		respond->set_respond_id(YUE_DU);
		emit sendCommand(network::MSG_RESPOND, respond);
		gui->reset();
		break;
	case AN_YUE_ZHAN:
		respond = new network::Respond();
		respond->set_src_id(myID);
		respond->add_args(1);
		foreach(Card*ptr, selectedCoverCards)
			respond->add_card_ids(ptr->getID());
		respond->set_respond_id(AN_YUE_ZHAN);
		emit sendCommand(network::MSG_RESPOND, respond);
		gui->reset();
		break;
	case CANG_BAI_ZHI_YUE:
		if (choice == 2)
			CangBaiZhiYue_2();
		else {
			action = newAction(ACTION_MAGIC_SKILL, CANG_BAI_ZHI_YUE);
			action->add_args(1);
			action->set_src_id(myID);
			emit sendCommand(network::MSG_ACTION, action);
			gui->reset();
		}
		break;
	case CANG_BAI_ZHI_YUE_2:
		action = newAction(ACTION_MAGIC_SKILL, CANG_BAI_ZHI_YUE);
		action->add_args(2);
		action->add_args(choice);
		action->add_dst_ids(selectedPlayers[0]->getID());
		action->set_src_id(myID);
		emit sendCommand(network::MSG_ACTION, action);
		gui->reset();
    }
    }catch(int error){
        logic->onError(error);
    }
}

void YueNv::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;

    static network::Respond* respond;

    switch(state)
    {
		case XIN_YUE_BI_HU:
		case MEI_DU_SHA:
		case YUE_ZHI_LUN_HUI:
		case YUE_DU:
		case AN_YUE_ZHAN:
			respond = new network::Respond();
			respond->set_src_id(myID);
			respond->add_args(0);
			respond->set_respond_id(state);
			emit sendCommand(network::MSG_RESPOND, respond);
			gui->reset();
			break;
		case MEI_DU_SHA_2:
			MeiDuSha(meidusha_card);
			break;
		case YUE_ZHI_LUN_HUI_2:
			YueZhiLunHui();
			break;
		case CANG_BAI_ZHI_YUE:
			normal();
			break;
		case CANG_BAI_ZHI_YUE_2:
			CangBaiZhiYue();
			break;
    }
}