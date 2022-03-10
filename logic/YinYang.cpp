#include "YinYang.h"

enum CAUSE{
	SHI_SHEN_ZHOU_SHU = 3301,
	SHI_SHEN_ZHOU_SHU_1 = 33011,
	SHI_SHEN_ZHOU_SHU_2 = 33012,
	SHI_SHEN_ZHOU_SHU_3 = 33013,
	HEI_AN_JI_LI = 3302,
	SHI_SHEN_JIANG_LIN = 3303,
	SHENG_MING_JIE_JIE = 3304,
	SHI_SHEN_ZHUAN_HUAN = 3305,
	YIN_YANG_ZHUAN_HUAN = 3306,
};

void YinYang::askForSkill(network::Command* cmd)
{
	if (cmd->respond_id() == HEI_AN_JI_LI) {
		HeiAnJiLi();
	}
	else if (cmd->respond_id() == SHI_SHEN_ZHOU_SHU) {
		ShiShenZhouShu(cmd->args(0), cmd->args(1));
	}
	else if (cmd->respond_id() == YIN_YANG_ZHUAN_HUAN) {
		YinYangZhuanHuan(cmd->args(0), cmd->args(1));
	}
	else{
		return Role::askForSkill(cmd);
	}
}

YinYang::YinYang()
{
    makeConnection();
    setMyRole(this);
	
	yinyangchange = new Button(10, QStringLiteral("阴阳转换"));
	buttonArea->addOutsideTurnButton(yinyangchange);
	yinyangchange->setVisible(true);
	yinyangchange->setEnabled(false);

	connect(yinyangchange, SIGNAL(buttonSelected(int)), this, SLOT(yinyangView()));
	connect(yinyangchange, SIGNAL(buttonUnselected(int)), this, SLOT(normalView()));

	shishenchange = new Button(11, QStringLiteral("式神转换"));
	buttonArea->addOutsideTurnButton(shishenchange);
	shishenchange->setVisible(true);
	shishenchange->setEnabled(false);
	connect(shishenchange, SIGNAL(buttonSelected(int)), this, SLOT(ShiShenon()));
	connect(shishenchange, SIGNAL(buttonUnselected(int)), this, SLOT(ShiShenoff()));

	Button *shengming, *jianglin;
	jianglin=new Button(3,QStringLiteral("式神降临"));
    buttonArea->addButton(jianglin);
    connect(jianglin,SIGNAL(buttonSelected(int)),this,SLOT(ShiShenJiangLin()));

    shengming=new Button(4,QStringLiteral("生命结界"));
    buttonArea->addButton(shengming);
    connect(shengming,SIGNAL(buttonSelected(int)),this,SLOT(ShengMingJieJie()));

}

void YinYang::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
	
	//式神降临
	if(!myself->getTap()) 
	{
	SafeList<Card*>handcards = dataInterface->getHandCards();
	for (int i = 0; i < handcards.size() - 1; i++)
		for (int j = i + 1; j < handcards.size(); j++)
			if (handcards[i]->getProperty() == handcards[j]->getProperty()) {
				buttonArea->enable(3);
				break;
			}
	}
	//生命结界
    if(myself->getEnergy())
    {
        buttonArea->enable(4);
    }
    unactionalCheck();
}

void YinYang::magicAction()
{
    Role::magicAction();
    Player* myself=dataInterface->getMyself();
	//式神降临
	if (!myself->getTap())
	{
		SafeList<Card*>handcards = dataInterface->getHandCards();
		for (int i = 0; i < handcards.size() - 1; i++)
			for (int j = i + 1; j < handcards.size(); j++)
				if (handcards[i]->getProperty() == handcards[j]->getProperty()) {
					buttonArea->enable(3);
					break;
				}
	}
	//生命结界
	if (myself->getEnergy())
	{
		buttonArea->enable(4);
	}
}
void YinYang::ShiShenJiangLin()
{
	state = SHENG_MING_JIE_JIE;
	tipArea->setMsg(QStringLiteral("式神降临：弃2张同命格（类别）手牌"));
	state = SHI_SHEN_JIANG_LIN;
	handArea->reset();
	playerArea->reset();
	tipArea->reset();

	playerArea->disableAll();
	handArea->enableAll();
	handArea->setQuota(2);

	decisionArea->enable(1);
	decisionArea->disable(0);
}

void YinYang::ShengMingJieJie()
{
	tipArea->setMsg(QStringLiteral("生命结界：选择目标队友+1宝石+1治疗"));
	state = SHENG_MING_JIE_JIE;
	handArea->reset();
	playerArea->reset();
	tipArea->reset();
	playerArea->enableMate();
	playerArea->setQuota(1);
	decisionArea->enable(1);
	decisionArea->disable(0);
}

void YinYang::cardAnalyse()
{
	SafeList<Card*> selectedCards;
	Role::cardAnalyse();
	try {
		switch (state)
		{
		case SHI_SHEN_JIANG_LIN:
			selectedCards = handArea->getSelectedCards();
			if (selectedCards.size() == 2 && (selectedCards[0]->getProperty() == selectedCards[1]->getProperty()))
				decisionArea->enable(0);
			else
				decisionArea->disable(0);
			break;
		case YIN_YANG_ZHUAN_HUAN:
		case SHI_SHEN_ZHOU_SHU:
			selectedCards = handArea->getSelectedCards();
			if (selectedCards[0]->getElement() == "light")
			{
				playerArea->reset();
				playerArea->setQuota(0);
				decisionArea->enable(0);
			}
			else
			{
				playerArea->setQuota(1);
				playerArea->enableEnemy();
				playerArea->disablePlayerItem(sourceID);
			}
			break;
		}
	}
	catch (int error) {
		logic->onError(error);
	}
}


void YinYang::YinYangZhuanHuan(int srcId, int cardId)
{
	state = YIN_YANG_ZHUAN_HUAN;
	gui->reset();
	Pro = (dataInterface->getCard(cardId)->getProperty());
	Ele = (dataInterface->getCard(cardId)->getElement());
	QString chEle = dataInterface->getCard(cardId)->getChineseElement();
	tipArea->setMsg(QStringLiteral("当前攻击为「")+chEle+ QStringLiteral("」系,「")+Pro+QStringLiteral("」命格，你可以发动 阴阳转换"));
	handArea->enableElement(Ele);
	handArea->enableElement("darkness");
	handArea->disableMagic();
	handArea->enableElement("light");
	handArea->setQuota(1);

	yinyangchange->setEnabled(true);
	yinyangchange->setSelected(false);
	shishenchange->setEnabled(true);
	shishenchange->setSelected(true);
	yinyangon = false;
	shishenon = true;
	decisionArea->enable(1);
	decisionArea->disable(0);
	sourceID = srcId;
}

void YinYang::yinyangView()
{
	handArea->reset();
	handArea->enableProperty(Pro);
	handArea->disableMagic();
	yinyangon = true;
	playerArea->reset();
	decisionArea->enable(1);
	decisionArea->disable(0);
}

void YinYang::normalView()
{
	handArea->reset();
	handArea->enableElement(Ele);
	handArea->enableElement("darkness");
	handArea->disableMagic();
	if(state == YIN_YANG_ZHUAN_HUAN)
		handArea->enableElement("light");
	yinyangon = false;
	playerArea->reset();
	decisionArea->enable(1);
	decisionArea->disable(0);
}
void YinYang::ShiShenon() { shishenon = true; }
void YinYang::ShiShenoff(){ shishenon = false; }


void YinYang::ShiShenZhouShu(int srcId, int cardId)
{
	state = SHI_SHEN_ZHOU_SHU;
	gui->reset();

	Pro = dataInterface->getCard(cardId)->getProperty();
	Ele = dataInterface->getCard(cardId)->getElement();
	QString chEle= dataInterface->getCard(cardId)->getChineseElement();
	tipArea->setMsg(QStringLiteral("当前攻击为「") + chEle + QStringLiteral("」系,「") + Pro + QStringLiteral("」命格，你可以发动 式神咒束"));
	handArea->enableElement(Ele);
	handArea->enableElement("darkness");
	handArea->disableMagic();
	handArea->setQuota(1);

	yinyangchange->setEnabled(true);
	yinyangchange->setSelected(false);
	shishenchange->setEnabled(true);
	shishenchange->setSelected(true);
	yinyangon = false;
	shishenon = true;

	tipArea->showBox();
	decisionArea->enable(1);
	decisionArea->disable(0);
	sourceID = srcId;
	atkCardId = cardId;
}

void YinYang::HeiAnJiLi()
{
	gui->reset();
	state = HEI_AN_JI_LI;
	Player* myself = dataInterface->getMyself();
	tipArea->setMsg(QStringLiteral("黑暗祭礼：选择目标角色，造成2点法术伤害"));
	if (myself->getTap())
	{
		tipArea->addBoxItem(QStringLiteral("1.保持【橫置】状态"));
		tipArea->addBoxItem(QStringLiteral("2.【重置】，退出状态"));
		tipArea->showBox();
	}

	decisionArea->disable(1);
	decisionArea->disable(0);
	playerArea->setQuota(1);
	playerArea->enableAll();
}

void YinYang::onOkClicked()
{
    Role::onOkClicked();
    SafeList<Card*> selectedCards;
    SafeList<Player*>selectedPlayers;
    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();
	Player* myself = dataInterface->getMyself();
    network::Action* action;
    network::Respond* respond;
	int choice=1;
	if (yinyangon)
	{
		choice++;
		if (shishenon)
			choice++;
	}
	try {
		switch (state)
		{
		case SHI_SHEN_JIANG_LIN:
			action = newAction(ACTION_MAGIC_SKILL, SHI_SHEN_JIANG_LIN);
			action->set_src_id(myID);
			action->add_card_ids(selectedCards[0]->getID());
			action->add_card_ids(selectedCards[1]->getID());
			emit sendCommand(network::MSG_ACTION, action);
			gui->reset();
			break;
		case SHENG_MING_JIE_JIE:
			action = newAction(ACTION_MAGIC_SKILL, SHENG_MING_JIE_JIE);
			action->set_src_id(myID);
			action->add_dst_ids(selectedPlayers[0]->getID());
			emit sendCommand(network::MSG_ACTION, action);
			gui->reset();
			break;
		case YIN_YANG_ZHUAN_HUAN:
			respond = new network::Respond();
			respond->set_src_id(myID);
			respond->add_card_ids(selectedCards[0]->getID());
			if(dataInterface->getCard( selectedCards[0]->getID())->getElement()!="light")
				respond->add_dst_ids(selectedPlayers[0]->getID());
			else
				respond->add_dst_ids(sourceID);
			respond->set_respond_id(YIN_YANG_ZHUAN_HUAN);
			respond->add_args(1);
			respond->add_args(choice);
			yinyangchange->setEnabled(false);
			yinyangchange->setSelected(false);
			shishenchange->setEnabled(false);
			shishenchange->setSelected(false);
			emit sendCommand(network::MSG_RESPOND, respond);
			break;
		case HEI_AN_JI_LI:
			respond = new network::Respond();
			respond->set_src_id(myID);
			respond->add_dst_ids(selectedPlayers[0]->getID());
			respond->set_respond_id(HEI_AN_JI_LI);
			if (myself->getTap())
				respond->add_args(tipArea->getBoxCurrentText()[0].digitValue());
			emit sendCommand(network::MSG_RESPOND, respond);
			break;
		case SHI_SHEN_ZHOU_SHU:
			respond = new network::Respond();
			respond->set_src_id(myID);
			respond->add_card_ids(selectedCards[0]->getID());
			respond->add_dst_ids(selectedPlayers[0]->getID());
			respond->set_respond_id(SHI_SHEN_ZHOU_SHU);
			respond->add_args(choice);
			yinyangchange->setEnabled(false);
			yinyangchange->setSelected(false);
			shishenchange->setEnabled(false);
			shishenchange->setSelected(false);
			emit sendCommand(network::MSG_RESPOND, respond);
			break;

    }

    }catch(int error){
        logic->onError(error);
    }
}

void YinYang::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;

    network::Respond* respond;

    switch(state)
    {
	case SHI_SHEN_JIANG_LIN:
	case SHENG_MING_JIE_JIE:
			normal();
		break;
	case YIN_YANG_ZHUAN_HUAN:
	case SHI_SHEN_ZHOU_SHU:
		respond = new network::Respond();
		respond->set_src_id(myID);
		respond->set_respond_id(state);
		respond->add_args(0);
		yinyangchange->setEnabled(false);
		yinyangchange->setSelected(false);
		shishenchange->setEnabled(false);
		shishenchange->setSelected(false);
		emit sendCommand(network::MSG_RESPOND, respond);
		gui->reset();
		break;
    }
}

