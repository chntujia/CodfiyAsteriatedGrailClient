#include "ShengGong.h"

enum CAUSE{
	TIAN_ZHI_GONG = 3701,
	SHENG_XIE_JU_BAO = 3702,
	SHENG_XIE_MISS = 37021,
	SHENG_HUANG_MO_SHI = 3703,
	SHENG_GUANG_BAO_LIE = 3704,
	SHENG_GUANG_BAO_LIE_2 = 37041,
	SHENG_LIU_XING_DAN = 3705,
	SHENG_LIU_XING_ATK = 37051,
	SHENG_LIU_XING_HIT = 37052,
	HUI_GUANG_PAO = 3706,
	ZI_DONG_TIAN_CHONG = 3707,
	ZI_DONG_TIAN_CHONG_2 = 37071,
};

ShengGong::ShengGong()
{
    makeConnection();
	setMyRole(this);
    Button *shengXie, *shengHuang, *shengGuang,*huiGuang;
    shengXie = new Button(3,QStringLiteral("圣屑飓暴"));
    buttonArea->addButton(shengXie);
    connect(shengXie,SIGNAL(buttonSelected(int)),this,SLOT(ShengXieJuBao()));

    shengHuang = new Button(4,QStringLiteral("圣煌模式"));
    buttonArea->addButton(shengHuang);
    connect(shengHuang,SIGNAL(buttonSelected(int)),this,SLOT(ShengHuangMoShi()));

    shengGuang = new Button(5,QStringLiteral("圣光爆裂"));
    buttonArea->addButton(shengGuang);
    connect(shengGuang,SIGNAL(buttonSelected(int)),this,SLOT(ShengGuangBaoLie()));

	huiGuang = new Button(6, QStringLiteral("圣煌辉光炮"));
	buttonArea->addButton(huiGuang);
	connect(huiGuang, SIGNAL(buttonSelected(int)), this, SLOT(HuiGuangPao()));

	mopai = new Button(10, QStringLiteral("摸牌"));
	buttonArea->addOutsideTurnButton(mopai);
	mopai->setVisible(true);
	mopai->setEnabled(false);

	connect(mopai, SIGNAL(buttonUnselected(int)), this, SLOT(QiPai()));

	qipai = new Button(12, QStringLiteral("弃牌"));
	buttonArea->addOutsideTurnButton(qipai);
	qipai->setVisible(true);
	qipai->setEnabled(false);
	connect(qipai, SIGNAL(buttonUnselected(int)), this, SLOT(MoPai()));
}

void ShengGong::normal()
{
	Role::normal();
	Player* myself = dataInterface->getMyself();

	SafeList<Card*>handcards = dataInterface->getHandCards();
	for (int i = 0; i < handcards.size() - 1; i++)
		for (int j = i + 1; j < handcards.size(); j++)
			if (handcards[i]->getElement() == handcards[j]->getElement()) {
				buttonArea->enable(3);
				break;
			}
	if ((myself->getToken(0) > 1 || myself->getCrossNum() > 1)
		&& !myself->getTap())
		buttonArea->enable(4);

	if (myself->getCrossNum() > 0 && myself->getTap()) buttonArea->enable(5);

	int color = myself->getColor();
	int myMorale, enMorale;
	if (color == 0) {
		myMorale = dataInterface->getBlueTeam()->getMorale();
		enMorale = dataInterface->getRedTeam()->getMorale();
	}
	else {
		enMorale = dataInterface->getBlueTeam()->getMorale();
		myMorale = dataInterface->getRedTeam()->getMorale();
	}
	if (myMorale < enMorale && myself->getToken(0) - 4 > enMorale - myMorale)
		buttonArea->enable(6);
	mopai->setEnabled(false);
	qipai->setEnabled(false);
	unactionalCheck();
}

void ShengGong::magicAction()
{
	Role::magicAction();
	Player* myself = dataInterface->getMyself();

	SafeList<Card*>handcards = dataInterface->getHandCards();
	for (int i = 0; i < handcards.size() - 1; i++)
		for (int j = i + 1; j < handcards.size(); j++)
			if (handcards[i]->getElement() == handcards[j]->getElement()) {
				buttonArea->enable(3);
				break;
			}
	if ((myself->getToken(0) > 1 || myself->getCrossNum() > 1)
		&& !myself->getTap())
		buttonArea->enable(4);

	if (myself->getCrossNum() > 0 && myself->getTap()) buttonArea->enable(5);
	int color = myself->getColor();
	int myMorale, enMorale;
	if (color == 0){
		myMorale = dataInterface->getBlueTeam()->getMorale();
		enMorale = dataInterface->getRedTeam()->getMorale();
	}
	else {
		enMorale = dataInterface->getBlueTeam()->getMorale();
		myMorale = dataInterface->getRedTeam()->getMorale();
	}
	if (myMorale < enMorale && myself->getToken(0) - 4 > enMorale - myMorale )
		buttonArea->enable(6);

	unactionalCheck();
}

void ShengGong::ShengXieJuBao()
{
	//借用底层的攻击目标判定，直到onOkClicked
	state = 10;
	shengxie = true;
	handArea->reset();
	playerArea->reset();
	tipArea->reset();
	tipArea->setMsg(QStringLiteral("选择攻击目标"));
	playerArea->setQuota(1);
	handArea->setQuota(2);

	decisionArea->enable(1);
	decisionArea->disable(0);

	handArea->enableAttack();
}

void ShengGong::ShengXieMiss()
{
	//借用底层的攻击目标判定，直到onOkClicked
	state = SHENG_XIE_MISS;
	gui->reset();
	QiPai();

	tipArea->setMsg(QStringLiteral("圣屑飓暴未命中：选择队友、使用治疗数、弃或摸牌"));
	Player* myself = dataInterface->getMyself();
	tipArea->setMsg(QStringLiteral("选择你要使用的治疗数"));
	int cross = myself->getCrossNum();
	for (int i = 1; i <= cross; i++)
		tipArea->addBoxItem(QString::number(i));
	tipArea->showBox();

	playerArea->enableMate();
	playerArea->setQuota(1);

	decisionArea->enable(1);
	decisionArea->disable(0);
}

void ShengGong::ShengHuangMoShi()
{
	state = SHENG_HUANG_MO_SHI;
	gui->reset();
	Player* myself = dataInterface->getMyself();
	tipArea->setMsg(QStringLiteral("选择发动方式"));
	if (myself->getCrossNum() > 1)
		tipArea->addBoxItem(QStringLiteral("1.移除2治疗"));
	if (myself->getToken(0) > 1)
		tipArea->addBoxItem(QStringLiteral("2.移除2信仰"));
	tipArea->showBox();
    decisionArea->enable(1);
    decisionArea->enable(0);
}

void ShengGong::ShengGuangBaoLie()
{
    state= SHENG_GUANG_BAO_LIE;
	gui->reset();
	Player* myself = dataInterface->getMyself();

	tipArea->setMsg(QStringLiteral("选择你要使用的治疗数"));
	int cross = myself->getCrossNum();
	for (int i=1; i <= cross; i++)
		tipArea->addBoxItem(QString::number(i));
	tipArea->showBox();
    
	decisionArea->enable(0);
    decisionArea->enable(1);
	
}

void ShengGong::ShengGuangBaoLie2()
{
	state = SHENG_GUANG_BAO_LIE_2;
	gui->reset();
	Player* myself = dataInterface->getMyself();
	if (moqi==2)
	{
		foreach(Player* ptr, dataInterface->getPlayerList())
		{
			if ( min((myself->getHandCardNum() + zhiliao),myself->getHandCardMax()) >= (ptr->getHandCardNum() )
				&& (ptr->getColor() != myself->getColor()))
				playerArea->enablePlayerItem(ptr->getID());
		}
		tipArea->setMsg(QStringLiteral("请选目标，当前为摸")+ QString::number(zhiliao)+QStringLiteral("张牌"));
		playerArea->setQuota(1, zhiliao);
		tipArea->showBox();
	}
	else if (moqi==1)
	{
		tipArea->setMsg(QStringLiteral("请先选弃牌再选目标，当前须弃") + QString::number(zhiliao) + QStringLiteral("张牌"));
		handArea->setQuota(zhiliao);
		handArea->enableAll();
		tipArea->showBox();
	}	
	decisionArea->disable(0);
	decisionArea->enable(1);
}


void ShengGong::ShengLiuXingAtk()
{
    state= SHENG_LIU_XING_ATK;
	gui->reset();
	Player* myself = dataInterface->getMyself();
	tipArea->setMsg(QStringLiteral("圣流星弹：选择发动方式和治疗目标"));
	if (myself->getCrossNum() > 0)
		tipArea->addBoxItem(QStringLiteral("1.移除1治疗"));
	if (myself->getToken(0) > 0)
		tipArea->addBoxItem(QStringLiteral("2.移除1信仰"));
	tipArea->showBox();
    decisionArea->disable(0);
    decisionArea->enable(1);

	handArea->disableAll();
	
	playerArea->setQuota(1);
	playerArea->enableMate(true);
}

void ShengGong::ZiDongTianChong()
{
	state = ZI_DONG_TIAN_CHONG;
	gui->reset();
	Player* myself = dataInterface->getMyself();
	tipArea->setMsg(QStringLiteral("自动填充：选择发动方式"));
	if (myself->getGem()>0){
		tipArea->addBoxItem(QStringLiteral("1.宝石：你+1水晶+2信仰"));
		tipArea->addBoxItem(QStringLiteral("2.宝石：你+1水晶+2治疗"));
	}
		tipArea->addBoxItem(QStringLiteral("3.水晶：你+1信仰"));
		tipArea->addBoxItem(QStringLiteral("4.水晶：你+1治疗"));
		tipArea->showBox();
	decisionArea->enable(0);
	decisionArea->disable(1);
}

void ShengGong::HuiGuangPao()
{
    state=HUI_GUANG_PAO;

	tipArea->setMsg(QStringLiteral("以何种方式使双方士气相等？"));
	tipArea->addBoxItem(QStringLiteral("1.增加我方士气"));
	tipArea->addBoxItem(QStringLiteral("2.扣减敌方士气"));
	tipArea->showBox();
    handArea->reset();
    playerArea->reset();

    decisionArea->enable(1);
    decisionArea->enable(0);
}

void ShengGong::MoPai()
{
	moqi = 2;
	qipai->setEnabled(false);
	mopai->setEnabled(true);
	mopai->setSelected(true);
	if (state == SHENG_GUANG_BAO_LIE_2)ShengGuangBaoLie2();
}

void ShengGong::QiPai()
{
	moqi = 1;
	mopai->setEnabled(false);
	qipai->setEnabled(true);
	qipai->setSelected(true);
	if (state == SHENG_GUANG_BAO_LIE_2)ShengGuangBaoLie2();
}

void ShengGong::cardAnalyse()
{
    Role::cardAnalyse();
	SafeList<Card*> selectedCards = handArea->getSelectedCards();
	int num = selectedCards.size();
	Player* myself = dataInterface->getMyself();
    try{
    switch (state)
    {
	case 10:
		foreach(Card*ptr, selectedCards)
			if (ptr->getElement() != selectedCards[0]->getElement()) {
				playerArea->reset();
				decisionArea->disable(0);
				break;
			}
		break;
	case SHENG_GUANG_BAO_LIE_2:
		if (num = zhiliao)
		{
			foreach(Player* ptr, dataInterface->getPlayerList())
			{
				if (max((myself->getHandCardNum() - zhiliao), 0) >= (ptr->getHandCardNum())
					&& (ptr->getColor() != myself->getColor()))
					playerArea->enablePlayerItem(ptr->getID());
			}
			playerArea->setQuota(1, zhiliao);
		}
		else
		{
			playerArea->reset();
		}
		break;
    }
    }catch(int error){
        logic->onError(error);
    }

}

void ShengGong::onOkClicked()
{
	if (shengxie && state == 10) {
		state = SHENG_XIE_JU_BAO;
	}

    Role::onOkClicked();
    SafeList<Card*> selectedCards;
    SafeList<Player*>selectedPlayers;

    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;
	QString ele;
	int choice = tipArea->getBoxCurrentText()[0].digitValue();
	if(selectedCards.size() > 0)
		ele = selectedCards[0]->getElement();
    try{
    switch(state)
    {
    case SHENG_XIE_JU_BAO:
		action = newAction(ACTION_MAGIC_SKILL, SHENG_XIE_JU_BAO);
		action->add_dst_ids(selectedPlayers[0]->getID());
		foreach(Card*ptr, selectedCards) {
			action->add_card_ids(ptr->getID());
		}
		if (ele == "earth")
			action->add_card_ids(54);
		else if (ele == "wind")
			action->add_card_ids(75);
		else if (ele == "water")
			action->add_card_ids(137);
		else if (ele == "thunder")
			action->add_card_ids(117);
		else if (ele == "fire")
			action->add_card_ids(96);
		else if (ele == "darkness")
			action->add_card_ids(39);
		action->add_args(choice);
		emit sendCommand(network::MSG_ACTION, action);
		
		shengxie = false;
		gui->reset();
        break;
    case SHENG_HUANG_MO_SHI:
        action = newAction(ACTION_MAGIC_SKILL, SHENG_HUANG_MO_SHI);
		action->add_args(choice);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case SHENG_GUANG_BAO_LIE:
		zhiliao = choice;
		gui->reset();		
		QiPai();
		ShengGuangBaoLie2();
		break;
	case SHENG_GUANG_BAO_LIE_2:
		action = newAction(ACTION_MAGIC_SKILL, SHENG_GUANG_BAO_LIE);
		if (moqi==1) {
			action->add_args(1);
			foreach(Card*ptr, selectedCards) {
				action->add_card_ids(ptr->getID());
			}
		}
		else
			action->add_args(2);
		action->add_args(zhiliao);
		foreach(Player*ptr, selectedPlayers)
			action->add_dst_ids(ptr->getID());
		emit sendCommand(network::MSG_ACTION, action);
		qipai->setEnabled(false);
		qipai->setSelected(false);
		mopai->setEnabled(false);
		mopai->setSelected(false);
		gui->reset();
		break;
    case HUI_GUANG_PAO:
        action = newAction(ACTION_MAGIC_SKILL, HUI_GUANG_PAO);
		action->add_args(choice);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
	case SHENG_LIU_XING_ATK:
		respond = newRespond(SHENG_LIU_XING_ATK);
		respond->add_dst_ids(selectedPlayers[0]->getID());
		respond->add_args(choice);
		emit sendCommand(network::MSG_RESPOND, respond);
		gui->reset();
		break;
	case ZI_DONG_TIAN_CHONG:
		respond = newRespond(ZI_DONG_TIAN_CHONG);
		respond->add_args(choice);
		emit sendCommand(network::MSG_RESPOND, respond);
		gui->reset();
		break;
	case SHENG_XIE_MISS:
		respond = newRespond(SHENG_XIE_MISS);
		respond->add_args(moqi);
		respond->add_args(choice);
		respond->add_dst_ids(selectedPlayers[0]->getID());
		emit sendCommand(network::MSG_RESPOND, respond);
		qipai->setEnabled(false);
		qipai->setSelected(false);
		mopai->setEnabled(false);
		mopai->setSelected(false);
		gui->reset();
		break;
    }

    }catch(int error){
        logic->onError(error);
    }
}


void ShengGong::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;

    network::Respond* respond;

    switch(state)
    {
	case SHENG_GUANG_BAO_LIE_2:
		ShengGuangBaoLie();
		break;
	case SHENG_XIE_MISS:
		qipai->setEnabled(false);
		mopai->setEnabled(false);
		respond = newRespond(state);
		respond->add_args(0);
		emit sendCommand(network::MSG_RESPOND, respond);
		gui->reset();
		break;
	case SHENG_LIU_XING_ATK:
		respond = newRespond(state);
		respond->add_args(0);
		emit sendCommand(network::MSG_RESPOND, respond);
		gui->reset();
		break;
	case 10:
    case SHENG_XIE_JU_BAO:
    case SHENG_HUANG_MO_SHI:
    case SHENG_GUANG_BAO_LIE:
	case HUI_GUANG_PAO:
        if(actionFlag==0)
            normal();
        else if(actionFlag==2)
            magicAction();
		shengxie = false;
        break;
    }
}
void ShengGong::askForSkill(network::Command* cmd)
{
    //Role::askForSkill(skill);
    if(cmd->respond_id() == SHENG_XIE_MISS)
	{
		ShengXieMiss();
	}
	else if(cmd->respond_id() == SHENG_LIU_XING_ATK)
	{
		ShengLiuXingAtk();
	}
	else if (cmd->respond_id() == ZI_DONG_TIAN_CHONG)
	{
		ZiDongTianChong();
	}
    else
	{
       	Role::askForSkill(cmd);
	}
}
