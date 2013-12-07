#include "QiDao.h"

enum CAUSE{
	QI_DAO = 1603,
	WEI_LI_CI_FU = 1602,
	XUN_JIE_CI_FU = 1601,
	GUANG_HUI_XIN_YANG = 1604,
	QI_HEI_XIN_YANG = 1605,
	FA_LI_CHAO_XI = 1606
};

void QiDao::askForSkill(network::Command* cmd)
{
    if(cmd->respond_id() == QI_DAO){
        QiDong();
	}else{
		return Role::askForSkill(cmd);
	}
}

QiDao::QiDao()
{
    makeConnection();
    setMyRole(this);

    Button *weiLi,*xunJie,*guangHui,*qiHei;
    weiLi=new Button(3,QStringLiteral("威力赐福"));
    buttonArea->addButton(weiLi);
    connect(weiLi,SIGNAL(buttonSelected(int)),this,SLOT(WeiLiCiFu()));

    xunJie=new Button(4,QStringLiteral("迅捷赐福"));
    buttonArea->addButton(xunJie);
    connect(xunJie,SIGNAL(buttonSelected(int)),this,SLOT(XunJieCiFu()));

    guangHui=new Button(5,QStringLiteral("光辉信仰"));
    buttonArea->addButton(guangHui);
    connect(guangHui,SIGNAL(buttonSelected(int)),this,SLOT(GuangHuiXinYang()));

    qiHei=new Button(6,QStringLiteral("漆黑信仰"));
    buttonArea->addButton(qiHei);
    connect(qiHei,SIGNAL(buttonSelected(int)),this,SLOT(QiHeiXinYang()));
}

void QiDao::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();

    if (handArea->checkSpecility(QStringLiteral("威力赐福")))
        buttonArea->enable(3);
    if (handArea->checkSpecility(QStringLiteral("迅捷赐福")))
        buttonArea->enable(4);
    if(myself->getTap()&&myself->getToken(0)>0)
    {
        buttonArea->enable(5);
        buttonArea->enable(6);
    }
    unactionalCheck();
}

void QiDao::magicAction()
{
    Role::magicAction();
    Player* myself=dataInterface->getMyself();

    if (handArea->checkSpecility(QStringLiteral("威力赐福")))
        buttonArea->enable(3);
    if (handArea->checkSpecility(QStringLiteral("迅捷赐福")))
        buttonArea->enable(4);
    if(myself->getTap()&&myself->getToken(0)>0)//getTap 返回是否横置 getToken返回标记物数目
    {
        buttonArea->enable(5);
        buttonArea->enable(6);
    }
}

void QiDao::QiDong()
{
    state=QI_DAO;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动祈祷？"));
    QList<Card*> handcards=dataInterface->getHandCards();


    int n=handcards.size();
    decisionArea->enable(1);
	bool flag=true;
    if(n<4)
    {
        flag=false;
        for(int i=0;i<n;i++){

            if(handcards[i]->getElement()!="light")
            {
                flag=true;
                break;
            }
		}
    }
    if(flag || dataInterface->getMyself()->getEnergy()>=1){
		 decisionArea->enable(0);
	}
}

void QiDao::WeiLiCiFu()
{
    state=WEI_LI_CI_FU;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableSpecility(QStringLiteral("威力赐福"));
}

void QiDao::XunJieCiFu()
{
    state=XUN_JIE_CI_FU;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableSpecility(QStringLiteral("迅捷赐福"));
}

void QiDao::GuangHuiXinYang()
{
    state=GUANG_HUI_XIN_YANG;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    int howMany=handArea->getHandCardItems().size();
    howMany=2<howMany?2:howMany;
    handArea->setQuota(howMany);
    handArea->enableAll();
    if(howMany==0)
        playerArea->enableMate();
    decisionArea->enable(1);
    decisionArea->disable(0);
}

void QiDao::QiHeiXinYang()
{
    state=QI_HEI_XIN_YANG;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    playerArea->enableAll();

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void QiDao::cardAnalyse()
{
    Role::cardAnalyse();
    QList<Player*>players=dataInterface->getPlayerList();
    switch (state)
    {
//威力赐福
    case WEI_LI_CI_FU:
        playerArea->enableMate();
        for(int i=0;i<players.size();i++)
           if(players[i]->checkBasicStatus(4))
               playerArea->disablePlayerItem(i);
        break;
//迅捷赐福
    case XUN_JIE_CI_FU:
        playerArea->enableMate();
        for(int i=0;i<players.size();i++)
           if(players[i]->checkBasicStatus(5))
               playerArea->disablePlayerItem(i);
        break;
//光辉信仰
    case GUANG_HUI_XIN_YANG:
        playerArea->enableMate();
        break;
    }
}

void QiDao::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;

    QString command;
    QString cardID;
    QString sourceID;
    QString targetID;
    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;

    switch(state)
    {
//额外行动询问
    case 42:
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1'){
            onceUsed=true;

            respond = new network::Respond();
			respond->set_src_id(myID);
            respond->set_respond_id(FA_LI_CHAO_XI);
            respond->add_args(1);
            emit sendCommand(network::MSG_RESPOND, respond);
            magicAction();

        }
        break;
//祈祷
    case QI_DAO:
        respond = new network::Respond();
		respond->set_src_id(myID);
        respond->set_respond_id(QI_DAO);
        respond->add_args(1);
		
        start = true;
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//威力赐福
    case WEI_LI_CI_FU:
        action = newAction(ACTION_MAGIC_SKILL, WEI_LI_CI_FU);
		action->set_src_id(myID);
		action->add_card_ids(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());

        //dataInterface->removeHandCard(selectedCards[0]);
        usedMagic=true;
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//迅捷赐福
    case XUN_JIE_CI_FU:
        action = newAction(ACTION_MAGIC_SKILL,XUN_JIE_CI_FU);
		action->set_src_id(myID);
        action->add_card_ids(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());

        //dataInterface->removeHandCard(selectedCards[0]);
        usedMagic=true;
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//光辉信仰
    case GUANG_HUI_XIN_YANG:
        action = newAction(ACTION_MAGIC_SKILL,GUANG_HUI_XIN_YANG);
		action->set_src_id(myID);
        foreach(Card*ptr,selectedCards){
            action->add_card_ids(ptr->getID());
            //dataInterface->removeHandCard(ptr);
        }
        action->add_dst_ids(selectedPlayers[0]->getID());

        usedMagic=true;
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//漆黑信仰
    case QI_HEI_XIN_YANG:
        action = newAction(ACTION_MAGIC_SKILL,QI_HEI_XIN_YANG);
		action->set_src_id(myID);
        action->add_args(1);
        action->add_dst_ids(selectedPlayers[0]->getID());

        usedMagic=true;
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    }
}

void QiDao::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;

    network::Respond* respond;

    switch(state)
    {
//祈祷
    case QI_DAO:
        respond = new network::Respond();
		respond->set_src_id(myID);
        respond->set_respond_id(QI_DAO);
        respond->add_args(0);

        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//威力赐福
    case WEI_LI_CI_FU:
//迅捷赐福
    case XUN_JIE_CI_FU:
//光辉信仰
    case GUANG_HUI_XIN_YANG:
//漆黑信仰
    case QI_HEI_XIN_YANG:
        if(actionFlag==0)
            normal();
        else if(actionFlag==2)
            magicAction();
        break;
    }
}

