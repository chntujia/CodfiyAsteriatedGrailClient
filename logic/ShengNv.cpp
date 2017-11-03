#include "ShengNv.h"

enum CAUSE{
    ZHI_LIAO_SHU = 601,
    ZHI_YU_ZHI_GUANG = 602,
    BING_SHUANG_DAO_YAN = 603,
    LIAN_MIN = 604,
    SHENG_LIAO = 605,
    SHENG_LIAO_1 = 651,
    SHENG_LIAO_2 = 652,
    SHENG_LIAO_3 = 653
};

ShengNv::ShengNv()
{
    makeConnection();
    setMyRole(this);
    Button *zhiLiaoShu, *zhiYuZhiGuang, *shengLiao;
    zhiLiaoShu = new Button(3,QStringLiteral("治疗术"));
    buttonArea->addButton(zhiLiaoShu);
    connect(zhiLiaoShu,SIGNAL(buttonSelected(int)),this,SLOT(ZhiLiaoShu()));

    zhiYuZhiGuang=new Button(4, QStringLiteral("治愈之光"));
    buttonArea->addButton(zhiYuZhiGuang);
    connect(zhiYuZhiGuang,SIGNAL(buttonSelected(int)),this,SLOT(ZhiYuZhiGuang()));

    shengLiao=new Button(5,QStringLiteral("圣疗"));
    buttonArea->addButton(shengLiao);
    connect(shengLiao,SIGNAL(buttonSelected(int)),this,SLOT(ShengLiao()));
}

void ShengNv::normal()
{
    Role::normal();
    Player* myself=dataInterface->getMyself();

    if (handArea->checkSpecility(QStringLiteral("治疗术")))
        buttonArea->enable(3);
    if (handArea->checkSpecility(QStringLiteral("治愈之光")))
        buttonArea->enable(4);
    //圣疗
    if (myself->getEnergy()>0 && !onceUsed)
        buttonArea->enable(5);
    unactionalCheck();
}


void ShengNv::attackOrMagic()
{
    Role::attackOrMagic();
    Player* myself=dataInterface->getMyself();
    if (handArea->checkSpecility(QStringLiteral("治疗术")))
        buttonArea->enable(3);
    if (handArea->checkSpecility(QStringLiteral("治愈之光")))
        buttonArea->enable(4);
    if (myself->getEnergy()>0 && !onceUsed)
        buttonArea->enable(5);
}


void ShengNv::ZhiLiaoShu()
{
    state=ZHI_LIAO_SHU;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableSpecility(QStringLiteral("治疗术"));
}

void ShengNv::ZhiYuZhiGuang()
{
    state=ZHI_YU_ZHI_GUANG;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();


    handArea->setQuota(1);
    playerArea->setQuota(1,3);
    decisionArea->enable(1);
    decisionArea->disable(0);

    handArea->enableSpecility(QStringLiteral("治愈之光"));
}

void ShengNv::LianMin()
{
    state=LIAN_MIN;
    gui->reset();
    tipArea->setMsg(QStringLiteral("是否发动怜悯？"));
    SafeList<Card*> handcards=dataInterface->getHandCards();
    bool flag=true;
    int i;
    int n=handcards.size();
    decisionArea->enable(1);
    if(n<4)
    {
        flag=false;
        for(i=0;i<n;i++)
            if(handcards[i]->getElement()!="light")
            {
                flag=true;
                break;
            }
            else if(handcards[i]->getProperty()==QStringLiteral("圣"))
            {
                flag=true;
                break;
            }
    }
    if(flag||dataInterface->getMyself()->getEnergy()>1)
        decisionArea->enable(0);
}
void ShengNv::ShengLiao()
{
    int gem, crystal;
    Player*myself=dataInterface->getMyself();
    gem=myself->getGem();
    crystal=myself->getCrystal();

    state=SHENG_LIAO_1;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();

    decisionArea->enable(1);
    decisionArea->disable(0);
    playerArea->setQuota(1);
    playerArea->enableAll();

    tipArea->setMsg(QStringLiteral("每次点选一人分配一点治疗，共三次"));
    if(crystal>=1)
        tipArea->addBoxItem(QStringLiteral("1.水晶"));
    if(gem>=1)
        tipArea->addBoxItem(QStringLiteral("2.宝石"));

    tipArea->showBox();
}
void ShengNv::ShengLiao2()
{
    state=SHENG_LIAO_2;
    onceUsed=true;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    buttonArea->reset();

    tipArea->setMsg(QStringLiteral("请选择分配第二点治疗"));
    playerArea->enableAll();
    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->disable(1);
    decisionArea->disable(0);

}
void ShengNv::ShengLiao3()
{
    state=SHENG_LIAO_3;
    handArea->reset();
    playerArea->reset();
    tipArea->reset();
    buttonArea->reset();

    tipArea->setMsg(QStringLiteral("请选择分配第三点治疗"));
    playerArea->enableAll();
    playerArea->setQuota(1);
    handArea->setQuota(1);

    decisionArea->disable(1);
    decisionArea->disable(0);

}
void ShengNv::BingShuangDaoYan()
{
    state=BING_SHUANG_DAO_YAN;

    tipArea->setMsg(QStringLiteral("请选择冰霜祷言的目标"));
    playerArea->enableAll();
    playerArea->setQuota(1);

    decisionArea->enable(1);
    decisionArea->disable(0);
}

void ShengNv::cardAnalyse()
{
    Role::cardAnalyse();
    switch (state)
    {
    case ZHI_LIAO_SHU:
    case ZHI_YU_ZHI_GUANG:
        playerArea->enableAll();
        break;
    }
}

void ShengNv::onOkClicked()
{
    Role::onOkClicked();
    SafeList<Card*> selectedCards;
    SafeList<Player*>selectedPlayers;

    QString text;

    selectedCards=handArea->getSelectedCards();
    selectedPlayers=playerArea->getSelectedPlayers();

    network::Action* action;
    network::Respond* respond;

    static int ShengLiao_count;
    static int ShengLiao_dst[3];
    static int ShengLiao_cross[3];
    try{
    switch(state)
    {
//额外行动询问
    case 42:
        text=tipArea->getBoxCurrentText();
        //攻击或法术行动
        if(text[0]=='1'){
            ShengLiaoAddition=false;
            respond = newRespond(606);
            respond->add_args(1);
            emit sendCommand(network::MSG_RESPOND, respond);
            attackOrMagic();
        }
        break;
//冰霜祷言
    case BING_SHUANG_DAO_YAN:
        respond = new Respond();
        respond->add_args(1);
        respond->add_dst_ids(selectedPlayers[0]->getID());
        respond->set_respond_id(BING_SHUANG_DAO_YAN);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//治疗术
    case ZHI_LIAO_SHU:
        action = newAction(ACTION_MAGIC_SKILL, ZHI_LIAO_SHU);
        action->add_card_ids(selectedCards[0]->getID());
        action->add_dst_ids(selectedPlayers[0]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//治愈之光
    case ZHI_YU_ZHI_GUANG:
        action = newAction(ACTION_MAGIC_SKILL, ZHI_YU_ZHI_GUANG);
        action->add_card_ids(selectedCards[0]->getID());
        for(int i=0; i<selectedPlayers.length(); i++)
            action->add_dst_ids(selectedPlayers[i]->getID());
        emit sendCommand(network::MSG_ACTION, action);
        gui->reset();
        break;
//怜悯
    case LIAN_MIN:
        respond = new Respond();
        respond->add_args(1);
        respond->set_respond_id(LIAN_MIN);
        start=true;
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//圣疗
    case SHENG_LIAO_1:
        ShengLiao_dst[0] = selectedPlayers[0]->getID();
        ShengLiao_count = 1;
        ShengLiao_cross[0] = 1;
        ShengLiao2();
        break;
    case SHENG_LIAO_2:
        if(ShengLiao_dst[0] == selectedPlayers[0]->getID())
        {
            ShengLiao_cross[0]++;
        }
        else
        {
            ShengLiao_dst[1] = selectedPlayers[0]->getID();
            ShengLiao_cross[1] = 1;
            ShengLiao_count++;
        }
        ShengLiao3();
        break;
    case SHENG_LIAO_3:
        int i;
        bool chosenPlayer = false;
        for(i=0;i<ShengLiao_count;i++)
        {
            if(ShengLiao_dst[i] == selectedPlayers[0]->getID())
            {
                ShengLiao_cross[i]++;
                chosenPlayer = true;
                break;
            }
        }
        if(!chosenPlayer)
        {
            ShengLiao_dst[i] = selectedPlayers[0]->getID();
            ShengLiao_cross[i] = 1;
            ShengLiao_count++;
        }

        action = newAction(ACTION_MAGIC_SKILL, SHENG_LIAO);
        for (int i = 0; i < ShengLiao_count; ++i)
        {
            action->add_dst_ids(ShengLiao_dst[i]);
            action->add_args(ShengLiao_cross[i]);
        }

        emit sendCommand(network::MSG_ACTION, action);
        ShengLiaoAddition=true;
        for(int i=0;i<3;i++)
        {
            ShengLiao_dst[i]=0;
            ShengLiao_cross[i]=0;
        }
        ShengLiao_count = 0;
        gui->reset();
        break;
    }

    }catch(int error){
        logic->onError(error);
    }
}


void ShengNv::onCancelClicked()
{
    Role::onCancelClicked();
    QString command;

    network::Respond* respond;

    switch(state)
    {
//怜悯
    case LIAN_MIN:
        respond = new Respond();
        respond->add_args(0);
        respond->set_respond_id(LIAN_MIN);
        emit sendCommand(network::MSG_RESPOND, respond);
        gui->reset();
        break;
//冰霜祷言
//治疗术
    case ZHI_LIAO_SHU:
//治愈之光
    case ZHI_YU_ZHI_GUANG:
//圣疗
    case SHENG_LIAO_1:
        if(actionFlag==0)
            normal();
        else if(actionFlag==4)
            attackOrMagic();
        break;
    }
}

void ShengNv::askForSkill(network::Command* cmd)
{
    switch (cmd->respond_id())
    {
    case BING_SHUANG_DAO_YAN:
        BingShuangDaoYan();
        break;
    case LIAN_MIN:
        LianMin();
        break;
    default:
        Role::askForSkill(cmd);
    }
}

void ShengNv::turnBegin()
{
    Role::turnBegin();
    ShengLiaoAddition = false;
}
