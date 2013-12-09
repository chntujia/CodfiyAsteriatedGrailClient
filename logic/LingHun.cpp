
#include "LingHun.h"

enum CAUSE{
        LING_HUN_ZHEN_BAO =2201,
        LING_HUN_CI_YU =2202,
        LING_HUN_ZENG_FU=2203,
        LING_HUN_TUN_SHI=2204,
        LING_HUN_ZHAO_HUAN=2205,
        LING_HUN_ZHUAN_HUAN=2206,
        LING_HUN_JING_XIANG=2207,
        LING_HUN_LIAN_JIE=2208,
        LING_HUN_LIAN_JIE_REACT=2209
};


LingHun::LingHun()
{

    makeConnection();
    setMyRole(this);
    lianJieUsed=false;

    Button *linghunzhaohuan;
    linghunzhaohuan=new Button(3,QStringLiteral("灵魂召还"));
    buttonArea->addButton(linghunzhaohuan);
    connect(linghunzhaohuan,SIGNAL(buttonSelected(int)),this,SLOT(LingHunZhaoHuan()));

    Button *linghunjingxiang;
    linghunjingxiang=new Button(4,QStringLiteral("灵魂镜像"));
    buttonArea->addButton(linghunjingxiang);
    connect(linghunjingxiang,SIGNAL(buttonSelected(int)),this,SLOT(LingHunJingXiang()));

    Button *linghunzhenbao;
    linghunzhenbao=new Button(5,QStringLiteral("灵魂震爆"));
    buttonArea->addButton(linghunzhenbao);
    connect(linghunzhenbao,SIGNAL(buttonSelected(int)),this,SLOT(LingHunZhenBao()));

    Button *linghunciyu;
    linghunciyu=new Button(6,QStringLiteral("灵魂赐予"));
    buttonArea->addButton(linghunciyu);
    connect(linghunciyu,SIGNAL(buttonSelected(int)),this,SLOT(LingHunCiYu()));
}

void LingHun::normal()
{
    Role::normal();
    Player *myself=dataInterface->getMyself();
    if (handArea->checkType("magic"))
        buttonArea->enable(3);
    if(myself->getToken(0)>=2)
        buttonArea->enable(4);
    if (handArea->checkSpecility(QStringLiteral("灵魂震爆")) && myself->getToken(0)>=3)
        buttonArea->enable(5);
    if (handArea->checkSpecility(QStringLiteral("灵魂赐予")) && myself->getToken(1)>=3)
        buttonArea->enable(6);
    unactionalCheck();
}

void LingHun::LingHunZhaoHuan()
{
    state= LING_HUN_ZHAO_HUAN;
    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    handArea->enableMagic();
    handArea->setQuota(1,7);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void LingHun::LingHunJingXiang()
{
    state=LING_HUN_JING_XIANG;
    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->disable(0);

    if(handArea->getHandCardItems().size()>3)
    {
        handArea->setQuota(3);
    }
    else
        handArea->setQuota(1,7);
    handArea->enableAll();
    playerArea->setQuota(1);
}

void LingHun::LingHunZhenBao()
{
    state=LING_HUN_ZHEN_BAO;
    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);

    handArea->enableSpecility(QStringLiteral("灵魂震爆"));
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

//
void LingHun::LingHunCiYu()
{
    state=LING_HUN_CI_YU;
    playerArea->reset();
    handArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);

    handArea->enableSpecility(QStringLiteral("灵魂赐予"));
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void LingHun::LingHunZhuanHuan()
{
    state=LING_HUN_ZHUAN_HUAN;
    Player *myself=dataInterface->getMyself();
    tipArea->setMsg(QStringLiteral("请选择要转换的灵魂："));
    decisionArea->enable(0);
    decisionArea->enable(1);

    if(myself->getToken(0)>0)
        tipArea->addBoxItem(QStringLiteral("1.将黄魂转化为蓝魂"));
    if(myself->getToken(1)>0)
        tipArea->addBoxItem(QStringLiteral("2.将蓝魂转化为黄魂"));
    tipArea->showBox();
}


//[灵魂链接]
void LingHun::LingHunLianJie()
{
    state=LING_HUN_LIAN_JIE;
    gui->reset();
    Player *myself=dataInterface->getMyself();
    decisionArea->enable(1);
    tipArea->setMsg(QStringLiteral("是否发动灵魂连接？如是请选择目标"));
    playerArea->enableMate();
    playerArea->setQuota(1);
}

//[灵魂链接] 转移伤害
void LingHun::LingHunLianJieReact(int harmPoint)
{
   state=LING_HUN_LIAN_JIE_REACT;
   gui->reset();
    Player *myself=dataInterface->getMyself();
   tipArea->reset();
   handArea->reset();
   playerArea->reset();
   decisionArea->enable(0);
   decisionArea->enable(1);
   tipArea->setMsg(QStringLiteral("请选择要转移的伤害："));
   int min=myself->getToken(1)<harmPoint?myself->getToken(1):harmPoint;
   for(;min>0;min--)                              //转移伤害数>0
    tipArea->addBoxItem(QString::number(min));
    tipArea->showBox();
}
// 【灵魂增幅】
void LingHun::LingHunZengFu()
{
    state=LING_HUN_ZENG_FU;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动灵魂增幅？"));
    decisionArea->enable(1);
    decisionArea->enable(0);
}

void LingHun::cardAnalyse()
{
    Role::cardAnalyse();
    switch(state)
    {
    case LING_HUN_ZHAO_HUAN:
        decisionArea->enable(0);
        break;
    case LING_HUN_ZHEN_BAO:
    case LING_HUN_CI_YU:
    case LING_HUN_JING_XIANG:
        playerArea->enableAll();
        break;
    }
}

void LingHun::onOkClicked()
{
    Role::onOkClicked();
    QList<Card*>selectedCards;
    QList<Player*>selectedPlayers;


    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;

    switch(state)
    {
    case LING_HUN_ZHAO_HUAN:
        action = newAction(ACTION_MAGIC_SKILL,LING_HUN_ZHAO_HUAN);
         for(int i=0;i<selectedCards.size();i++)
             action->add_card_ids(selectedCards[i]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
        
     case  LING_HUN_JING_XIANG:
        action = newAction(ACTION_MAGIC_SKILL,LING_HUN_JING_XIANG);
        foreach(Card*ptr,selectedCards){
             action->add_card_ids(ptr->getID());
        }
        action->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;

    case LING_HUN_ZHEN_BAO:
        action = newAction(ACTION_MAGIC_SKILL,LING_HUN_ZHEN_BAO);
        action->add_card_ids(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case LING_HUN_CI_YU:
        action = newAction(ACTION_MAGIC_SKILL,LING_HUN_CI_YU);
        action->add_card_ids(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
    case LING_HUN_ZHUAN_HUAN:
        respond = new Respond();
        respond->set_src_id(myID);
        respond->set_respond_id(LING_HUN_ZHUAN_HUAN);

        text=tipArea->getBoxCurrentText();
        if(text[0].digitValue()==1)
            respond->add_args(0);
        else
            respond->add_args(1);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case LING_HUN_LIAN_JIE:
            respond = new Respond();
            respond->set_respond_id(LING_HUN_LIAN_JIE);
            respond->set_src_id(myID);
            respond->add_dst_ids(selectedPlayers[0]->getID());
            respond->add_args(1);
            emit sendCommand(network::MSG_RESPOND, respond);
            gui->reset();
        break;
    case LING_HUN_LIAN_JIE_REACT:
            respond = new Respond();
            respond->set_respond_id(LING_HUN_LIAN_JIE_REACT);
            respond->set_src_id(myID);
            respond->add_args(tipArea->getBoxCurrentText().toInt());
            emit sendCommand(network::MSG_RESPOND, respond);
            gui->reset();
        break;
    case LING_HUN_ZENG_FU:
        respond = new Respond();
        respond->set_respond_id(LING_HUN_ZENG_FU);
        respond->set_src_id(myID);
    //  respond->set_respond_id(skillCmd.respond_id());
        respond->add_args(1);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
}

void LingHun::onCancelClicked()
{
    Role::onCancelClicked();

    network::Respond* respond;

    switch(state)
    {
    case LING_HUN_ZHEN_BAO:
    case LING_HUN_JING_XIANG:
    case LING_HUN_ZHAO_HUAN:
    case LING_HUN_CI_YU:
        normal();
        break;
    case LING_HUN_ZHUAN_HUAN:
        respond = new Respond();
        respond->set_src_id(myID);
        respond ->set_respond_id(LING_HUN_ZHUAN_HUAN);
        respond->add_args(2);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;

    case LING_HUN_LIAN_JIE:
        respond = new Respond();
        respond->set_respond_id(LING_HUN_LIAN_JIE);
        respond->set_src_id(myID);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case LING_HUN_LIAN_JIE_REACT:
        respond = new Respond();
        respond->set_respond_id(LING_HUN_LIAN_JIE_REACT);
        respond->set_src_id(myID);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    case LING_HUN_ZENG_FU:
        respond = new Respond();
        respond->set_src_id(myID);
        respond ->set_respond_id(LING_HUN_ZENG_FU);
        respond->add_args(0);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
    }
}

void LingHun::askForSkill(network::Command* cmd)
{
     //灵魂链接稍后补上
    if(cmd->respond_id() == LING_HUN_ZHUAN_HUAN)
        LingHunZhuanHuan();
    else if(cmd->respond_id() == LING_HUN_ZENG_FU)
        LingHunZengFu();
    else if(cmd->respond_id() == LING_HUN_LIAN_JIE)
        LingHunLianJie();
    else if(cmd->respond_id() == LING_HUN_LIAN_JIE_REACT)
        LingHunLianJieReact(cmd->args(0));
    else
        Role::askForSkill(cmd);
}
