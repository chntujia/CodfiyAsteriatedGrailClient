#include "HongLian.h"

enum CAUSE{
    XING_HONG_SHENG_YUE = 2801,
    XING_HONG_XIN_YANG = 2802,
	XUE_XING_DAO_YAN = 2803,
    XUE_XING_DAO_YAN_1 = 2804,
	XUE_XING_DAO_YAN_2 = 2805,
	SHA_LU_SHENG_YAN = 2806,
	RE_XUE_FEI_TENG = 2807,
	JIE_JIAO_JIE_ZAO = 2808,
	JIE_JIAO_JIE_ZAO_AFTER_MAGIC = 28081,
	XING_HONG_SHI_ZI =2809
};

HongLian::HongLian()
{
    makeConnection();
    setMyRole(this);
    Button *xingHongShiZi;
    xingHongShiZi = new Button(3,QStringLiteral("腥红十字"));
    buttonArea->addButton(xingHongShiZi);
    connect(xingHongShiZi,SIGNAL(buttonSelected(int)),this,SLOT(XingHongShiZi()));
}

void HongLian::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    SafeList<Card*> handcards=dataInterface->getHandCards();
    int magic = 0;
    for(int i=0; i<handcards.size();i++)
    {
        if(handcards[i]->getType() == QStringLiteral("magic"))
            magic++;
    }
    if(magic>1 && myself->getToken(0)>0 && myself->getEnergy()>0)
        buttonArea->enable(3);
    unactionalCheck();
}

void HongLian::XueXingDaoYan()
{
    state =XUE_XING_DAO_YAN;
    tipArea->setMsg(QStringLiteral("是否发动血腥祷言？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void HongLian::XueXingDaoYan1()
{
    state = XUE_XING_DAO_YAN_1;
    gui->reset();
    Player* myself=dataInterface->getMyself();

    tipArea->setMsg(QStringLiteral("【血腥祷言】请选择分给第一人的治疗"));
    for(int i = 1;i<=myself->getCrossNum();i++)
        tipArea->addBoxItem(QString::number(i));
    tipArea->showBox();

    playerArea->enableMate();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void HongLian::XueXingDaoYan2()
{
    state = XUE_XING_DAO_YAN_2;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    Player* myself=dataInterface->getMyself();
    tipArea->setMsg(QStringLiteral("【血腥祷言】请选择分给第二人的治疗或直接确认"));
    for(int i = 1;i<=(myself->getCrossNum()-cross[0]);i++)
        tipArea->addBoxItem(QString::number(i));
    tipArea->showBox();
    playerArea->enableMate();
    playerArea->disablePlayerItem(dst[0]);
    playerArea->setQuota(1);
    decisionArea->enable(1);
    decisionArea->enable(0);
}

void HongLian::XingHongShiZi()
{
    state = XING_HONG_SHI_ZI;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();    

    handArea->setQuota(2);
    handArea->enableMagic();
    playerArea->setQuota(1);
    decisionArea->disable(0);
    decisionArea->enable(1);
}

void HongLian::cardAnalyse()
{
    Role::cardAnalyse();
    switch (state)
    {
    case XING_HONG_SHI_ZI:
        playerArea->enableAll();
        break;
    }
}

void HongLian::onOkClicked()
{
    Role::onOkClicked();
    SafeList<Card*> selectedCards;
    SafeList<Player*>selectedPlayers;

    selectedCards = handArea->getSelectedCards();
    selectedPlayers = playerArea->getSelectedPlayers();
    Player* myself=dataInterface->getMyself();

    network::Action* action;
    network::Respond* respond;

    static int DaoYan_count;
    static int DaoYan_dst[2];
    static int DaoYan_num[2];
    try{
    switch(state)
    {
    case XUE_XING_DAO_YAN_1:
        cross[0] = tipArea->getBoxCurrentText().toInt();
		if(myself->getCrossNum()>cross[0] && 6 == playerList.size())
        {
            DaoYan_dst[0] = selectedPlayers[0]->getID();
			dst[0] = DaoYan_dst[0];
            DaoYan_num[0] = tipArea->getBoxCurrentText().toInt();
			cross[0] = DaoYan_num[0];
            DaoYan_count = 1;
            XueXingDaoYan2();
        }
        else
        {
            respond = new network::Respond();
            respond->set_src_id(myID);
            respond->set_respond_id(XUE_XING_DAO_YAN);
            respond->add_dst_ids(selectedPlayers[0]->getID());
            respond->add_args(tipArea->getBoxCurrentText().toInt());

            start = true;
            emit sendCommand(network::MSG_RESPOND, respond);
			dst[0] = 0;
			cross[0] = 0;
        }
        break;
    case XUE_XING_DAO_YAN_2:
        if(selectedPlayers.size()>0)
        {
            DaoYan_dst[DaoYan_count] = selectedPlayers[0]->getID();
            DaoYan_num[DaoYan_count] = tipArea->getBoxCurrentText().toInt();
            DaoYan_count += 1;
        }
        start = true;

        respond = new network::Respond();
		respond->set_src_id(myID);
		respond->set_respond_id(XUE_XING_DAO_YAN);
        for (int i = 0; i < DaoYan_count; ++i)
        {
            respond->add_dst_ids(DaoYan_dst[i]);
            respond->add_args(DaoYan_num[i]);
        }

        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
		dst[0] = 0;
		cross[0] = 0;
        break;
    case XING_HONG_SHI_ZI:
		action = newAction(ACTION_MAGIC_SKILL, XING_HONG_SHI_ZI);
        action->add_dst_ids(selectedPlayers[0]->getID());

        foreach(Card*ptr,selectedCards){
            action->add_card_ids(ptr->getID());
            //dataInterface->removeHandCard(ptr);
        }
        gui->reset();
        emit sendCommand(network::MSG_ACTION, action);
        break;
    }
    }catch(int error){
        logic->onError(error);
    }
}

void HongLian::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;

    static network::Respond* respond;

    switch(state)
    {
    case XUE_XING_DAO_YAN_1:
        respond = newRespond(XUE_XING_DAO_YAN);
		respond->add_args(0);
        respond->set_src_id(myID);
        emit sendCommand(network::MSG_RESPOND, respond);
        break;
    case XUE_XING_DAO_YAN_2:
		dst[0] = 0;
		cross[0] = 0;
        XueXingDaoYan1();
        break;
    case XING_HONG_SHI_ZI:
        if(0 == actionFlag)
            normal();
        else if(4 == actionFlag)
            attackOrMagic();
        break;
    }
}

void HongLian::attackOrMagic()
{
    Role::attackOrMagic();
    Player* myself=dataInterface->getMyself();
    SafeList<Card*> handcards=dataInterface->getHandCards();
    int magic = 0;
    for(int i=0; i<handcards.size();i++)
    {
        if(handcards[i]->getType() == QStringLiteral("magic"))
            magic++;
    }
    if(magic>1 && myself->getToken(0)>0 && myself->getEnergy()>0)
        buttonArea->enable(3);
}

void HongLian::askForSkill(network::Command* cmd)
{
    //Role::askForSkill(skill);
	switch (cmd->respond_id())
    {
		case XUE_XING_DAO_YAN:
			XueXingDaoYan1();
			break;
		default:
			Role::askForSkill(cmd);
    }
}


