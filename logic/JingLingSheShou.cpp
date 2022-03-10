#include "JingLingSheShou.h"

enum CAUSE {
	YUAN_SU_SHE_JI = 3201,
	YUAN_SU_SHE_JI_HIT = 32011,
	DONG_WU_HUO_BAN = 3202,
	JING_LING_MI_YI = 3203,
	JING_LING_MI_YI_BOOT = 32031,
};

JingLingSheShou::JingLingSheShou()
{
	makeConnection();
	setMyRole(this);
}

void JingLingSheShou::askForSkill(network::Command* cmd)
{
	//判断服务器的响应请求
	if (cmd->respond_id() == YUAN_SU_SHE_JI) {
		YuanSuSheJi();
	}
	else if (cmd->respond_id() == DONG_WU_HUO_BAN) {
		DongWuHuoBan();
	}
	else if (cmd->respond_id() == JING_LING_MI_YI_BOOT) {
		JingLingMiYiBoot();
	}
	else if (cmd->respond_id() == JING_LING_MI_YI) {
		JingLingMiYi();
	}
	else if (cmd->respond_id() == YUAN_SU_SHE_JI_HIT) {
		YuanSuSheJiHit(cmd);
	}
	else {//for other skill{
		Role::askForSkill(cmd);
	}
}

void JingLingSheShou::YuanSuSheJi()
{
	gui->reset();
	state = YUAN_SU_SHE_JI;
	tipArea->setMsg(QStringLiteral("是否发动元素射击？"));
	handArea->reset();
	handArea->setQuota(1);
	handArea->enableMagic();
	foreach(CardItem *ptr1, coverArea->getHandCardItems())
	{
		foreach(CardItem *ptr2, handArea->getHandCardItems())
			if (ptr2->getCard()->getID() == ptr1->getCard()->getID())
			{
				ptr2->setEnabled(1);
				ptr2->setOpacity(1);
			 }
	}
	decisionArea->enable(1);
	decisionArea->disable(0);
}

void JingLingSheShou::YuanSuSheJiHit(network::Command* cmd)
{
	gui->reset();
	state = YUAN_SU_SHE_JI_HIT;
	if (cmd->args(0) == 1)
	{
		tipArea->setMsg(QStringLiteral("元素射击——水：请选择治疗目标"));
	}
	else if (cmd->args(0) == 2)
	{
		tipArea->setMsg(QStringLiteral("元素射击——地：请选择法伤目标"));
	}
	playerArea->enableAll();
	playerArea->setQuota(1);
	decisionArea->disable(1);
	decisionArea->disable(0);

}

void JingLingSheShou::cardAnalyse()
{
	SafeList<Card*> selectedCards;
	Role::cardAnalyse();
	try {
		switch (state)
		{
		case YUAN_SU_SHE_JI:
			decisionArea->enable(0);
			break;
		}
	}
	catch (int error) {
		logic->onError(error);
	}
}

void JingLingSheShou::drop(int howMany, int cause)
{
	Role::drop(howMany,cause);
	foreach(CardItem *ptr1, coverArea->getHandCardItems())
	{
		foreach(CardItem *ptr2, handArea->getHandCardItems())
			if (ptr2->getCard()->getID() == ptr1->getCard()->getID())
			{
				ptr2->setEnabled(0);
				ptr2->setOpacity(0.8);
			}
	}
}

void JingLingSheShou::DongWuHuoBan()
{
	state = DONG_WU_HUO_BAN;
	handArea->reset();
	playerArea->reset();
	tipArea->reset();
	Player* myself = dataInterface->getMyself();
	if (myself->getEnergy() > 0)
	{
		playerArea->setQuota(1);
		playerArea->enableAll();
		tipArea->setMsg(QStringLiteral("是否发动动物伙伴？指定自己以外的角色将消耗能量"));
	}
	else
		tipArea->setMsg(QStringLiteral("是否发动动物伙伴？"));
	decisionArea->enable(1);
	decisionArea->enable(0);
}
void JingLingSheShou::JingLingMiYiBoot() 
{
	gui->reset();
	state = JING_LING_MI_YI_BOOT;
	tipArea->setMsg(QStringLiteral("是否发动精灵密仪？"));
	decisionArea->enable(1);
	decisionArea->enable(0);
}
void JingLingSheShou::JingLingMiYi() 
{
	gui->reset();
	state = JING_LING_MI_YI;
	tipArea->setMsg(QStringLiteral("重置：选择目标角色，造成2点法术伤害"));
	decisionArea->disable(1);
	decisionArea->disable(0);
	playerArea->setQuota(1);
	playerArea->enableAll();
}

void JingLingSheShou::onOkClicked()
{
	Role::onOkClicked();

	SafeList<Card*> selectedCards = handArea->getSelectedCards();
	SafeList<Player*> selectedPlayers = playerArea->getSelectedPlayers();
	int a = 1;
	network::Respond* respond;
	try {
		switch (state)
		{
			//元素射击
		case YUAN_SU_SHE_JI:
			respond = new network::Respond();
			respond->set_src_id(myID);
			respond->set_respond_id(YUAN_SU_SHE_JI);
			respond->add_card_ids(selectedCards[0]->getID());
			if(coverArea->getHandCardItems().size()>0)
			{
				foreach(CardItem *ptr, coverArea->getHandCardItems())
				{
					if (ptr->getCard()->getID() == selectedCards[0]->getID())
					{ 
					a=2;
					break;
					}
				}
			}
			respond->add_args(a);
			emit sendCommand(network::MSG_RESPOND, respond);
			break;
		case YUAN_SU_SHE_JI_HIT:
			respond = new network::Respond();
			respond->set_src_id(myID);
			respond->set_respond_id(YUAN_SU_SHE_JI_HIT);
			respond->add_dst_ids(selectedPlayers[0]->getID());
			emit sendCommand(network::MSG_RESPOND, respond);
			break;
			//动物伙伴
		case DONG_WU_HUO_BAN:
			if (selectedPlayers.size() > 0)//宠物强化
			{
				respond = new network::Respond();
				respond->set_src_id(myID);
				respond->add_dst_ids(selectedPlayers[0]->getID());
				respond->set_respond_id(DONG_WU_HUO_BAN);
				respond->add_args(1);
				emit sendCommand(network::MSG_RESPOND, respond);
			}
			else//不点人默认自己
			{
				respond = new network::Respond();
				respond->set_src_id(myID);
				respond->add_dst_ids(myID);
				respond->set_respond_id(DONG_WU_HUO_BAN);
				respond->add_args(1);
				emit sendCommand(network::MSG_RESPOND, respond);
			}
			gui->reset();
			break;
			//精灵密仪-启动
		case JING_LING_MI_YI_BOOT:
			respond = new network::Respond();
			respond->set_src_id(myID);
			respond->set_respond_id(JING_LING_MI_YI_BOOT);
			respond->add_args(1);
			emit sendCommand(network::MSG_RESPOND, respond);
			start = true;
			break;
			//精灵密仪-回合结束
		case JING_LING_MI_YI:
			respond = new network::Respond();
			respond->set_src_id(myID);
			respond->add_dst_ids(selectedPlayers[0]->getID());
			respond->set_respond_id(JING_LING_MI_YI);
			respond->add_args(1);
			emit sendCommand(network::MSG_RESPOND, respond);
			break;
		}
	}
	catch (int error) {
		logic->onError(error);
	}
}

void JingLingSheShou::onCancelClicked()
{
	Role::onCancelClicked();
	network::Respond* respond;
	switch (state)
	{
		//元素射击
	case YUAN_SU_SHE_JI:
		//动物伙伴
	case DONG_WU_HUO_BAN:
		//精灵密仪-启动
	case JING_LING_MI_YI_BOOT:
		respond = new network::Respond();
		respond->set_src_id(myID);
		respond->set_respond_id(state);
		respond->add_args(0);// 0表示不选择发动
		emit sendCommand(network::MSG_RESPOND, respond);
		gui->reset();
		break;
	}
}