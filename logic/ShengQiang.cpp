#include "shengqiang.h"

enum CAUSE{
    HUI_YAO = 1001,
    CHENG_JIE = 1002,
    SHENG_GUANG_QI_YU = 1003,
	TIAN_QIANG = 1004,
	DI_QIANG = 1005,
	SHEN_SHENG_XIN_YANG = 1006,
	SHENG_JI = 1007
};

ShengQiang::ShengQiang()
{
    makeConnection();
	setMyRole(this);
    Button *huiYao, *chengJie, *shengGuangQiYu;
    huiYao = new Button(3,QStringLiteral("辉耀"));
    buttonArea->addButton(huiYao);
    connect(huiYao,SIGNAL(buttonSelected(int)),this,SLOT(HuiYao()));

    chengJie = new Button(4,QStringLiteral("惩戒"));
    buttonArea->addButton(chengJie);
    connect(chengJie,SIGNAL(buttonSelected(int)),this,SLOT(ChengJie()));

    shengGuangQiYu = new Button(5,QStringLiteral("圣光祈愈"));
    buttonArea->addButton(shengGuangQiYu);
    connect(shengGuangQiYu,SIGNAL(buttonSelected(int)),this,SLOT(ShengGuangQiYu()));
}

void ShengQiang::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();
    if(handArea->checkElement("water"))
        buttonArea->enable(3);
    bool chengjieflag = false;
    SafeList<Player*>players=dataInterface->getPlayerList();
    for(int i=0;i<players.size();i++)
    {
        if(myself->getID()==players[i]->getID())
            continue;
        if(players[i]->getCrossNum()>0)
        {
            chengjieflag = true;
            break;
        }
    }
    if(handArea->checkType("magic")&&chengjieflag)
        buttonArea->enable(4);
    if(myself->getGem()>0)
        buttonArea->enable(5);
    unactionalCheck();
}

void ShengQiang::HuiYao()
{
    state=HUI_YAO;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableElement("water");
}

void ShengQiang::ChengJie()
{
    state=CHENG_JIE;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableMagic();

}

void ShengQiang::TianQiang()
{
    state=36;
    tipArea->setMsg(QStringLiteral("是否发动天枪？"));
    decisionArea->enable(0);
    decisionArea->enable(1);
	handArea->disableAll();
}

void ShengQiang::DiQiang()
{
    state=DI_QIANG;
    tipArea->setMsg(QStringLiteral("请选择发动地枪使用的治疗点数，取消将发动圣击"));
    decisionArea->enable(0);
    decisionArea->enable(1);
	handArea->disableAll();
    Player* myself=dataInterface->getMyself();
	tipArea->reset();
    int cross = myself->getCrossNum();
    if (cross>4)
        cross = 4;
    for(;cross>0;cross--)
        tipArea->addBoxItem(QString::number(cross));
    tipArea->showBox();
}

void ShengQiang::ShengGuangQiYu()
{
    state=SHENG_GUANG_QI_YU;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->enable(0);
}

void ShengQiang::cardAnalyse()
{
    Role::cardAnalyse();
    try{
    switch (state)
    {
    case HUI_YAO:
        decisionArea->enable(0);
        break;
    case CHENG_JIE:
        Player* myself=dataInterface->getMyself();
        SafeList<Player*>players=dataInterface->getPlayerList();
        for(int i=0;i<players.size();i++)
        {
            if(myself->getID()==players[i]->getID())
                continue;
            if(players[i]->getCrossNum()>0)
                playerArea->enablePlayerItem(i);
        }
        break;
    }
    }catch(int error){
        logic->onError(error);
    }

}

void ShengQiang::onOkClicked()
{
    Role::onOkClicked();
    SafeList<Card*> selectedCards;
    SafeList<Player*>selectedPlayers;

    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;
    try{
    switch(state)
    {
    case 42:
        text=tipArea->getBoxCurrentText();
        if(text[0]=='1'){
            HuiYaoAddition=false;
            respond = newRespond(1003);
            respond->add_args(1);
            emit sendCommand(network::MSG_RESPOND, respond);
            attackAction();
        }
        else if(text[0]=='2'){
            ChengJieAddition=false;
            respond = newRespond(1004);
            respond->add_args(1);
            emit sendCommand(network::MSG_RESPOND, respond);
            attackAction();
        }
        else if(text[0]=='3'){
            ShengGuangQiYuAddition=false;
            respond = newRespond(1007);
            respond->add_args(1);
            emit sendCommand(network::MSG_RESPOND, respond);
            attackAction();
        }
        break;
    case HUI_YAO:
		action = newAction(ACTION_MAGIC_SKILL, HUI_YAO);
        action->add_card_ids(selectedCards[0]->getID());

        HuiYaoAddition=true;
        //dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case CHENG_JIE:
        action = newAction(ACTION_MAGIC_SKILL, CHENG_JIE);
        action->add_card_ids(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());
        //dataInterface->removeHandCard(selectedCards[0]);
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case DI_QIANG:
		respond = new Respond();
        respond->add_args(tipArea->getBoxCurrentText().toInt());
		respond->set_respond_id(DI_QIANG);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case SHENG_GUANG_QI_YU:
        action = newAction(ACTION_MAGIC_SKILL, SHENG_GUANG_QI_YU);
        //action->add_args(1);
        ShengGuangQiYuAddition=true;
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    }

    }catch(int error){
        logic->onError(error);
    }
}

void ShengQiang::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;

    network::Respond* respond;

    switch(state)
    {
    case DI_QIANG:
        respond = new Respond();
		respond->add_args(0);
		respond->set_respond_id(DI_QIANG);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case HUI_YAO:
    case CHENG_JIE:
    case SHENG_GUANG_QI_YU:
        if(actionFlag==0)
            normal();
        else if(actionFlag==1)
            attackAction();
        break;
    }
}
void ShengQiang::askForSkill(network::Command* cmd)
{
    //Role::askForSkill(skill);
    if(cmd->respond_id() == TIAN_QIANG)
	{
		TianQiang();
	}
	else if(cmd->respond_id() == DI_QIANG)
	{
		DiQiang();
	}
    else
	{
       	Role::askForSkill(cmd);
	}
}

void ShengQiang::turnBegin()
{
    Role::turnBegin();
    HuiYaoAddition =false;
    ChengJieAddition=false;
    ShengGuangQiYuAddition=false;
}
