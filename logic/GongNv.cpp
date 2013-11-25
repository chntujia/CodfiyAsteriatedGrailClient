#include "GongNv.h"

enum CAUSE{
	JING_ZHUN_SHE_JI = 301,
	SHAN_GUANG_XIAN_JIN = 302,
	JU_JI = 303,
	SHAN_DIAN_JIAN = 304,
	GUAN_CHUAN_SHE_JI = 305
};
GongNv::GongNv()
{
    makeConnection();
	setMyRole(this);

    Button *shanGuangXianJing,*juJi;
    shanGuangXianJing=new Button(3,QStringLiteral("闪光陷阱"));
    buttonArea->addButton(shanGuangXianJing);
    connect(shanGuangXianJing,SIGNAL(buttonSelected(int)),this,SLOT(ShanGuangXianJing()));

    juJi=new Button(4,QStringLiteral("狙击"));
    buttonArea->addButton(juJi);
    connect(juJi,SIGNAL(buttonSelected(int)),this,SLOT(JuJi()));
}

void GongNv::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
	//闪光陷阱
    if (handArea->checkSpecility(QStringLiteral("闪光陷阱"))){
        buttonArea->enable(3);
	}
	//狙击
    if(myself->getEnergy()>0)
        buttonArea->enable(4);
    unactionalCheck();
}

void GongNv::askForSkill(network::Command* cmd)
{
    //判断服务器的响应请求
    if(cmd->respond_id() == GUAN_CHUAN_SHE_JI){
        GuanChuanSheJi();
	}
    else if(cmd->respond_id() == JING_ZHUN_SHE_JI){
        JingZhunSheJi();
	}
	else{//for dibug{
		  
	}
}



void GongNv::GuanChuanSheJi()
{
    state=305;
    tipArea->setMsg(QStringLiteral("是否发动贯穿射击？"));
	handArea->reset();
    handArea->setQuota(1);
    handArea->enableMagic();
    decisionArea->enable(1);
	//decisionArea->enable(0);
}

void GongNv::JingZhunSheJi()
{
    state=301;
    tipArea->setMsg(QStringLiteral("是否发动精准射击？"));
	handArea->disableAll();
    decisionArea->enable(0);
    decisionArea->enable(1);
}

void GongNv::ShanGuangXianJing()
{
    state=302;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableSpecility(QStringLiteral("闪光陷阱"));
}

void GongNv::JuJi()
{
    int gem,crystal;
    Player*myself=dataInterface->getMyself();

    gem=myself->getGem();
    crystal=myself->getCrystal();

    state=303;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->disable(0);
    playerArea->setQuota(1);
    playerArea->enableAll();

   
}

void GongNv::cardAnalyse()
{
    Role::cardAnalyse();

    switch (state)
    {
	case 305:
        decisionArea->enable(0);
        break;
    case 302:
        playerArea->enableAll();
        break;
    }
}


void GongNv::onOkClicked()
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
            respond = new network::Respond();
            respond->set_src_id(myID);
            respond->set_respond_id(304);
            respond->add_args(1);

            emit sendCommand(network::MSG_RESPOND, respond);
            JuJiAdditon=false;
            attackAction();
        }
        break;
//贯穿询问
    case 305:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(305);
		respond->add_card_ids(selectedCards[0]->getID());
		respond->add_args(1);// 1表示选择发动
        //dataInterface->removeHandCard(selectedCards[0]);移牌由客户端完成
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;   
//闪光陷阱
    case 302:
        action = newAction(ACTION_MAGIC_SKILL, 302);
        action->set_src_id(myID);
        action->set_action_id(302);

        action->add_card_ids(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());

        //dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//狙击
    case 303:
        action = newAction(ACTION_MAGIC_SKILL, 303);
        action->set_src_id(myID);
        action->set_action_id(303);
        action->add_dst_ids(selectedPlayers[0]->getID());

        JuJiAdditon=true;
        text=tipArea->getBoxCurrentText();
        action->add_args(1);
		
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//精准射击	
	case 301:
        respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(301);
		respond->add_args(1);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
}


void GongNv::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;

    network::Respond* respond;

    switch(state)
    {
	//精准射击
	case 301:
        respond = new network::Respond();
		respond->set_src_id(myID);
		respond->set_respond_id(301);
		respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

//贯穿询问
    case 305:
		respond = new network::Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(305);
		respond->add_args(0);// 0表示不选择发动
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//闪光陷阱
    case 302:
//狙击
    case 303:
        normal();
        break;
    }
}
