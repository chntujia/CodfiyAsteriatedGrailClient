#include "XueJian.h"

enum CAUSE {
	XUE_SE_JING_JI = 3401,
	CHI_SE_YI_SHAN = 3402,
	XUE_RAN_QIANG_WEI = 3403,
	XUE_RAN_QIANG_WEI_1 = 34031,
	XUE_RAN_QIANG_WEI_2 = 34032,
	XUE_QI_PING_ZHANG = 3404,
	XUE_QIANG_WEI_BOOT = 3405,
	XUE_QIANG_WEI_BOOT_2 = 34051
};

XueJian::XueJian()
{
    makeConnection();
	setMyRole(this);
    Button *XueRanQiangWei;
	XueRanQiangWei =new Button(3,QStringLiteral("血染蔷薇"));
    buttonArea->addButton(XueRanQiangWei);
    connect(XueRanQiangWei,SIGNAL(buttonSelected(int)),this,SLOT(XueRanQiangWei1()));
}

void XueJian::askForSkill(network::Command* cmd)
{
	//Role::askForSkill(skill);
	switch (cmd->respond_id())
	{
	case XUE_QI_PING_ZHANG:
		XueQiPingZhang();
		break;
	case XUE_QIANG_WEI_BOOT:
		XueQiangWeiBoot();
		break;
	default:
		Role::askForSkill(cmd);
	}
}

void XueJian::XueRanQiangWei1()
{
    state=XUE_RAN_QIANG_WEI_1;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    tipArea->setMsg(QStringLiteral("请选择任意目标，移除治疗"));
	playerArea->enableAll();
	playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void XueJian::XueRanQiangWei2()
{
    state= XUE_RAN_QIANG_WEI_2;
	XueRanTargetID =playerArea->getSelectedPlayers().takeFirst()->getID();

	handArea->reset();
	playerArea->reset();
	tipArea->reset();

	tipArea->setMsg(QStringLiteral("请选择任意我方角色，其1水晶将变为宝石"));
	playerArea->enableMate(true);
	playerArea->setQuota(1);

	decisionArea->enable(1);
	decisionArea->disable(0);
}

void XueJian::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
	//血染蔷薇
	if (myself->getToken(0) >1)
		buttonArea->enable(3);
    unactionalCheck();
}

void XueJian::magicAction()
{
	Role::normal();
	Player* myself = dataInterface->getMyself();
	//血染蔷薇
	if (myself->getToken(0) > 1)
		buttonArea->enable(3);
}

void XueJian::XueQiPingZhang()
{
	state = XUE_QI_PING_ZHANG;
	gui->reset();
	tipArea->setMsg(QStringLiteral("若发动血气屏障，请选择目标"));
	playerArea->enableEnemy();
	playerArea->setQuota(1);
	decisionArea->enable(1);
	decisionArea->disable(0);
}

void XueJian::XueQiangWeiBoot()
{
	state = XUE_QIANG_WEI_BOOT;
	gui->reset();
	tipArea->setMsg(QStringLiteral("血蔷薇庭院：若发动请选择效果"));
	Player *myself = dataInterface->getMyself();
	if(myself->getGem()>0)
		tipArea->addBoxItem(QStringLiteral("1.宝石：无视上限+2鲜血（至多4），弃至4牌"));
	tipArea->addBoxItem(QStringLiteral("2.水晶：+2鲜血"));
	
	tipArea->showBox();
	decisionArea->enable(0);
	decisionArea->enable(1);
}

void XueJian::XueQiangWeiBoot2()
{
	state = XUE_QIANG_WEI_BOOT_2;
	gui->reset();
	tipArea->setMsg(QStringLiteral("血蔷薇庭院（宝石）：选择弃牌"));
	handArea->enableAll();
	handArea->setQuota(dataInterface->getHandCards().size() - 4);
	decisionArea->enable(1);
	decisionArea->disable(0);
}

void XueJian::cardAnalyse()
{
	Role::cardAnalyse();
	switch (state)
	{
	case XUE_QIANG_WEI_BOOT_2:
			decisionArea->enable(0);
		break;
	}
}

void XueJian::onOkClicked()
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
//血染蔷薇
    case XUE_RAN_QIANG_WEI_1:
        XueRanQiangWei2();
        break;
    case XUE_RAN_QIANG_WEI_2:
		action = newAction(ACTION_MAGIC_SKILL, XUE_RAN_QIANG_WEI);
        action->add_dst_ids(XueRanTargetID);
        action->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
		XueRanTargetID = 0;
        gui->reset();
        break;
//血气屏障
    case XUE_QI_PING_ZHANG:
		respond = new network::Respond();
		respond->set_src_id(myID);
		respond->set_respond_id(XUE_QI_PING_ZHANG);
		respond->add_dst_ids(selectedPlayers[0]->getID());
		respond->add_args(1);
		emit sendCommand(network::MSG_RESPOND, respond);
        break;
//血蔷薇庭院
    case XUE_QIANG_WEI_BOOT:
		if (tipArea->getBoxCurrentText()[0].digitValue() == 1)
		{
			if (dataInterface->getHandCards().size() > 4)
				XueQiangWeiBoot2();
			else
			{
				respond = new network::Respond();
				respond->set_src_id(myID);
				respond->set_respond_id(XUE_QIANG_WEI_BOOT);
				respond->add_args(1);
				emit sendCommand(network::MSG_RESPOND, respond);
				start = true;
				gui->reset();
			}
		}
		else
		{
			respond = new network::Respond();
			respond->set_src_id(myID);
			respond->set_respond_id(XUE_QIANG_WEI_BOOT);
			respond->add_args(2);
			emit sendCommand(network::MSG_RESPOND, respond);
			start = true;
			gui->reset();
		}
        break;
	case XUE_QIANG_WEI_BOOT_2:
		respond = new network::Respond();
		respond->set_src_id(myID);
		respond->set_respond_id(XUE_QIANG_WEI_BOOT);
		respond->add_args(1);
		foreach(Card*ptr, selectedCards) {
			respond->add_card_ids(ptr->getID());
		}
		emit sendCommand(network::MSG_RESPOND, respond);
		start = true;
		gui->reset();
		break;
    }

    }catch(int error){
        logic->onError(error);
    }
}

void XueJian::onCancelClicked()
{
    Role::onCancelClicked();
	QString command;
	network::Respond* respond;
    switch(state)
    {
    case XUE_RAN_QIANG_WEI_1:
		normal();
		break;
    case XUE_RAN_QIANG_WEI_2:
		XueRanQiangWei1();
		break;
    case XUE_QI_PING_ZHANG:
	case XUE_QIANG_WEI_BOOT:
		respond = new network::Respond();
		respond->set_src_id(myID);
		respond->set_respond_id(state);
		respond->add_args(0);
		emit sendCommand(network::MSG_RESPOND, respond);
		gui->reset();
        break;
	case XUE_QIANG_WEI_BOOT_2:
        XueQiangWeiBoot();
        break;
    }
}
