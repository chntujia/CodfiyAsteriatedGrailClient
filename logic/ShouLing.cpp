#include "ShouLing.h"
enum CAUSE{
	WU_ZHE_CAN_XIN = 3601,
	YI_JI_WU_NIAN = 3602,
	YI_JI_ATK = 36021,
	SHOU_HUN_YI_NIAN = 3603,
	SHOU_HUN_JING_JIE = 3604,
	SHOU_FAN = 3605,
	SHOU_FAN_1 = 36051,
	NI_FAN_ZHAN = 3606,
	NI_FAN_ATK = 36061,
	NI_FAN_HIT = 36062,
	YU_HUN_BOOT = 3607,
	YU_HUN_BOOT_1 = 360711,
	YU_HUN_BOOT_2 = 360712,
	YU_HUN_ATK = 36071
};

ShouLing::ShouLing()
{
    makeConnection();
    setMyRole(this);
}

void ShouLing::askForSkill(network::Command* cmd)
{
	//Role::askForSkill(skill);
	switch (cmd->respond_id())
	{
	case SHOU_HUN_JING_JIE:
		ShouHunJingJie();
		break;
	case SHOU_FAN:
		ShouFan();
		break;
	case NI_FAN_HIT:
		NiFanHit();
		break;
	case YU_HUN_BOOT:
		YuHunBoot();
		break;
	default:
		Role::askForSkill(cmd);
	}
}

void ShouLing::ShouHunJingJie()
{
	state = SHOU_HUN_JING_JIE;
	gui->reset();
	tipArea->setMsg(QStringLiteral("兽魂警戒：请选择目标弃牌"));
	playerArea->enableAll();
	playerArea->setQuota(1);
	decisionArea->enable(1);
	decisionArea->disable(0);
}

void ShouLing::ShouFan()
{
	state = SHOU_FAN;
	gui->reset();
	tipArea->setMsg(QStringLiteral("兽返：选择移除兽魂数量"));

	Player* myself = dataInterface->getMyself();
	int num = myself->getToken(1);
	for (; num > 0; num--)
	{
		tipArea->addBoxItem(QString::number(num));
	}
	tipArea->showBox();
	decisionArea->enable(1);
	decisionArea->enable(0);
}

void ShouLing::ShouFan1()
{
	state = SHOU_FAN_1;
	gui->reset();
	tipArea->setMsg(QStringLiteral("兽返：选择弃牌"));
	int handnum = dataInterface->getHandCards().size();
	handArea->enableAll();
	if(handnum > shoufannum)
		handArea->setQuota(shoufannum);
	else
		handArea->setQuota(handnum);

	decisionArea->disable(0);
	decisionArea->enable(1);
}

void ShouLing::cardAnalyse()
{
	Role::cardAnalyse();
	switch (state)
	{
	case SHOU_FAN_1:
		decisionArea->enable(0);
		break;
	case YU_HUN_BOOT_1:
		decisionArea->enable(0);
		break;
	}
}

void ShouLing::NiFanHit()
{
	state = NI_FAN_HIT;
	gui->reset();
	tipArea->setMsg(QStringLiteral("逆反居合斩（命中）；额外移除兽魂？"));
	Player* myself = dataInterface->getMyself();
	int num = myself->getToken(1);
	for (; num > 0; num--)
	{
		tipArea->addBoxItem(QString::number(num));
	}
	tipArea->showBox();
	decisionArea->enable(0);
	decisionArea->enable(1);
}

void ShouLing::YuHunBoot()
{
	state =YU_HUN_BOOT;
	gui->reset();
	tipArea->setMsg(QStringLiteral("是否启动 御魂流居合式"));
	
	if (dataInterface->getHandCards().size() > 0) {
		tipArea->addBoxItem(QStringLiteral("1.弃1张牌"));
	}
	
	tipArea->addBoxItem(QStringLiteral("2.摸1张牌"));

	if (dataInterface->getHandCards().size() > 0) {
		tipArea->addBoxItem(QStringLiteral("3.启动但不调整"));
	}
	tipArea->showBox();

	decisionArea->enable(0);
	decisionArea->enable(1);
}

void ShouLing::YuHunBoot1()
{
	state = YU_HUN_BOOT_1;
	gui->reset();
	tipArea->setMsg(QStringLiteral("御魂流居合式：选择弃牌"));
	handArea->enableAll();
	handArea->setQuota(1);
	decisionArea->disable(0);
	decisionArea->enable(1);
}


void ShouLing::onOkClicked()
{
	Role::onOkClicked();
	SafeList<Card*> selectedCards;
	SafeList<Player*>selectedPlayers;
	selectedCards = handArea->getSelectedCards();
	selectedPlayers = playerArea->getSelectedPlayers();
	network::Respond* respond;
	int choice = tipArea->getBoxCurrentText()[0].digitValue();
	int num = 0;
	try {
		switch (state)
		{
			//兽魂警戒
		case SHOU_HUN_JING_JIE:
			respond = new network::Respond();
			respond->set_src_id(myID);
			respond->set_respond_id(SHOU_HUN_JING_JIE);
			respond->add_dst_ids(selectedPlayers[0]->getID());
			respond->add_args(1);
			emit sendCommand(network::MSG_RESPOND, respond);
			gui->reset();
			break;
			//兽返
		case SHOU_FAN:
			shoufannum = choice;
			if (dataInterface->getHandCards().size() > 0)
			{
				
				ShouFan1();
			}
			else
			{
				respond = new network::Respond();
				respond->set_src_id(myID);
				respond->set_respond_id(SHOU_FAN);
				respond->add_args(1);
				respond->add_args(shoufannum);
				shoufannum = 0;
				emit sendCommand(network::MSG_RESPOND, respond);
				gui->reset();
			}
			break;
		case SHOU_FAN_1:
			respond = new network::Respond();
			respond->set_src_id(myID);
			respond->set_respond_id(SHOU_FAN);
			respond->add_args(1);
			respond->add_args(shoufannum);
			foreach(Card*ptr, selectedCards) {
				respond->add_card_ids(ptr->getID());
			}
			shoufannum = 0;
			emit sendCommand(network::MSG_RESPOND, respond);
			gui->reset();
			break;
			//逆反居合命中
		case NI_FAN_HIT:
			respond = new network::Respond();
			respond->set_src_id(myID);
			respond->set_respond_id(NI_FAN_HIT);
			respond->add_args(1);
			respond->add_args(choice);
			emit sendCommand(network::MSG_RESPOND, respond);
			gui->reset();
			break;
			//御魂流居合式
		case YU_HUN_BOOT:
			if (choice == 1)
			{
				YuHunBoot1();
			}
			else if(choice == 2 )
			{
				respond = new network::Respond();
				respond->set_src_id(myID);
				respond->set_respond_id(YU_HUN_BOOT);
				respond->add_args(2);
				respond->add_args(1);
				emit sendCommand(network::MSG_RESPOND, respond);
				start = true;
				gui->reset();
			}
			else {
				respond = new network::Respond();
				respond->set_src_id(myID);
				respond->set_respond_id(YU_HUN_BOOT);
				respond->add_args(3);
				respond->add_args(0);
				emit sendCommand(network::MSG_RESPOND, respond);
				start = true;
				gui->reset();
			}
			break;
		case YU_HUN_BOOT_1:
			respond = new network::Respond();
			respond->set_src_id(myID);
			respond->set_respond_id(YU_HUN_BOOT);
			respond->add_args(1);
			respond->add_card_ids(selectedCards[0]->getID());
			respond->add_args(1);
			emit sendCommand(network::MSG_RESPOND, respond);
			start = true;
			gui->reset();
			break;
		}

	}
	catch (int error) {
		logic->onError(error);
	}
}

void ShouLing::onCancelClicked()
{
	Role::onCancelClicked();
	QString command;
	network::Respond* respond;
	switch (state)
	{
	case SHOU_FAN_1:
		ShouFan();
		break;
	case YU_HUN_BOOT_1:
	case YU_HUN_BOOT_2:
		YuHunBoot();
		break;
	case SHOU_HUN_JING_JIE:
	case SHOU_FAN:
	case NI_FAN_HIT:
	case YU_HUN_BOOT:
		respond = new network::Respond();
		respond->set_src_id(myID);
		respond->set_respond_id(state);
		respond->add_args(0);
		emit sendCommand(network::MSG_RESPOND, respond);
		gui->reset();
		break;
	}
}