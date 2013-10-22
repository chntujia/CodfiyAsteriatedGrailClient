#include "logic/Logic.h"
#include "data/DataInterface.h"
#include "Role.h"
#include "widget/PlayerArea.h"
#include "JianSheng.h"
#include "KuangZhan.h"
#include "GongNv.h"
#include "FengYin.h"
#include "AnSha.h"
#include "ShengNv.h"
#include "TianShi.h"
#include "MoDao.h"
#include "MoJian.h"
#include "YuanSu.h"
#include "MaoXian.h"
#include "YongZhe.h"
#include "ZhongCai.h"
#include "ShengQiang.h"
#include "QiDao.h"
#include "ShenGuan.h"
#include "SiLing.h"
#include "XianZhe.h"
#include "JianDi.h"
#include "GeDouJia.h"
#include "WuNv.h"
#include "DieWu.h"
#include "LingHun.h"
#include "HongLian.h"
#include "MoQiang.h"
#include "LingFu.h"
#include "MoGong.h"

#define LOGINPERMIT 1
#define GAMESTART 2

Logic* logic=NULL;
Logic::Logic(QObject *parent) :
    QObject(parent)
{
    count=0;
    hasShownRole=false;
    hasSetRole=false;
    state = -1;
    memset(roles,-1,sizeof(roles));
}
void Logic::readyToStart()
{
    network::StartReply *start_reply = new network::StartReply();
    emit sendCommand(network::MSG_START_REP, start_reply);
}
void Logic::setMyRole(int roleID)
{
    if(hasSetRole)
        return;
    switch(roleID)
    {
    case 1:
        new JianSheng;
        hasSetRole = true;
        break;
    case 2:
        new KuangZhan;
        hasSetRole = true;
        break;
    case 3:
        new GongNv;
        hasSetRole = true;
        break;
    case 4:
        new FengYin;
        hasSetRole = true;
        break;
    case 5:
        new AnSha;
        hasSetRole = true;
        break;
    case 6:
        new ShengNv;
        hasSetRole = true;
        break;
    case 7:
        new TianShi;
        hasSetRole = true;
        break;
    case 8:
        new MoDao;
        hasSetRole = true;
        break;
    case 9:
        new MoJian;
        hasSetRole = true;
        break;
    case 10:
        new ShengQiang;
        hasSetRole = true;
        break;
    case 11:
        new YuanSu;
        hasSetRole = true;
        break;
    case 12:
        new MaoXian;
        hasSetRole = true;
        break;
    case 13:
        new SiLing;
        hasSetRole = true;
        break;
    case 14:
        new ZhongCai;
        hasSetRole = true;
        break;
    case 15:
        new ShenGuan;
        hasSetRole = true;
        break;
    case 16:
        new QiDao;
        hasSetRole = true;
        break;
    case 17:
        new XianZhe;
        hasSetRole = true;
        break;
    case 18:
        new LingFu;
        hasSetRole = true;
        break;
    case 19:
        new JianDi;
        hasSetRole = true;
        break;
    case 20:
        new GeDouJia;
        hasSetRole = true;
        break;
    case 21:
        new YongZhe;
        hasSetRole = true;
        break;
    case 22:
        new LingHun;
        hasSetRole = true;
        break;
    case 23:
        new WuNv;
        hasSetRole = true;
        break;
    case 24:
        new DieWu;
        hasSetRole = true;
        break;
    case 26:
        new MoGong;
        hasSetRole = true;
        break;
    case 28:
        new HongLian;
        hasSetRole = true;
        break;
    case 29:
        new MoQiang;
        hasSetRole = true;
        break;
    }

}

void Logic::getCommand(uint16_t proto_type, google::protobuf::Message* proto)
{
    TipArea *tipArea;
    DecisionArea* decisionArea;
    BPArea* bpArea;
    QList<int> roleIDs;
    PlayerArea* playerArea;
    QStringList nicknames;
    int playerMax,targetID,roleID,howMany,num;
    QString temp;
    QString arg[10];

    network::CharactorPickRequest* char_pick;

    switch (proto_type)
    {
    //Port Redirect
    case 150:
        /*
        temp=socket->GetAddr();
        socket->disconnectFromHost();
        socket->connectToHost(temp, arg[1].toInt());
        socket->forwarded=true;
        emit sendCommand("0;"+QString::number(socket->isRed)+";"+socket->nickname+";");
        */
        break;
    case network::MSG_SINGLE_ROOM:
        myID=((network::SingleRoom*)proto)->player_id();
        dataInterface->setID(myID);
        break;
    case network::MSG_GAME:
        if(state == -1){
            state = GAMESTART;
            network::GameInfo* game_info = (network::GameInfo*) proto;

            playerMax = game_info->player_infos_size();
            dataInterface->setPlayerMax(playerMax);
            //nicknames=arg[2].split(',');
            dataInterface->initPlayerList(game_info);
            if (playerMax==8)
                dataInterface->initTeam(18);
            else
                dataInterface->initTeam(15);

            //dataInterface初始化完成
            emit gameStart();
        }
        else {
            //角色宣告
            network::GameInfo* game_info = (network::GameInfo*) proto;
            for (int i = 0; i < game_info->player_infos_size(); ++i)
            {
                network::SinglePlayerInfo* player_info = (network::SinglePlayerInfo*)&(game_info->player_infos(i));

                targetID=player_info->id();
                roleID=player_info->role_id();

                roles[targetID]=roleID;
            }
            count++;
            if(count==dataInterface->getPlayerMax())
            {
                disconnect(getClient(),0,this,0);

                for(int i=0;i<dataInterface->getPlayerMax();i++)
                    dataInterface->getPlayerList().at(i)->setRole(roles[i]);

                for(int i=0;i<dataInterface->getPlayerMax();i++){
                    gui->getPlayerArea()->getPlayerItem(i)->setToolTip(dataInterface->getRoleSkillInfo(roles[i]));
                }

                roleID = roles[myID];
                dataInterface->getPlayerList().at(myID)->setRole(roleID);
                setMyRole(roleID);
                gui->getPlayerArea()->getPlayerItem(myID)->setToolTip(dataInterface->getRoleSkillInfo(roleID));

                gui->getPlayerArea()->update();
            }
        }
        break;
    case network::MSG_GOSSIP:
        if(gui!=NULL)
        {
            network::Gossip* gossip = (network::Gossip*) proto;
            gui->logAppend(QString::fromStdString(gossip->txt()));
        }
        break;
//选择角色
    case network::MSG_PICK_REQ:
        state=46;
        char_pick = (network::CharactorPickRequest*) proto;

        tipArea=gui->getTipArea();
        decisionArea=gui->getDecisionArea();
        tipArea->reset();
        connect(decisionArea,SIGNAL(okClicked()),this,SLOT(onOkClicked()));
        howMany=char_pick->role_ids_size();
        for(int i=0;i<howMany;i++){
            roleID=char_pick->role_ids(i);
            tipArea->addBoxItem(QString::number(roleID)+"."+dataInterface->getRoleName(roleID));
        }
        tipArea->setMsg(QStringLiteral("请选择角色："));
        tipArea->showBox();
        decisionArea->enable(0);
        break;
    case 51:
        // TODO:BP模式，暂不可用
        state = 51;
        tipArea=gui->getTipArea();
        decisionArea=gui->getDecisionArea();
        bpArea = gui->getBPArea();
        tipArea->reset();
        connect(decisionArea,SIGNAL(okClicked()),this,SLOT(onOkClicked()));
        connect(bpArea,SIGNAL(roleReady()),this,SLOT(roleAnalyse()));
        num = arg[1].toInt();

        for(int i=0;i<num;i++)
        {
            roleIDs<<arg[i+2].toInt();
        }

        bpArea->BPStart(num, roleIDs);
        break;
    case 52:
        // TODO:BP模式，暂不可用
        state = 52;
        bpArea = gui->getBPArea();
        bpArea->setMsg("请禁用一角色");
        bpArea->setQuota(1);
        bpArea->enableAll();
        playerArea = gui->getPlayerArea();
        gui->alert();
        break;
    case 55:
        // TODO:BP模式，暂不可用
        state = 55;
        bpArea = gui->getBPArea();
        bpArea->setMsg("请选择一角色");
        bpArea->setQuota(1);
        bpArea->enableAll();
        playerArea = gui->getPlayerArea();
        gui->alert();
        break;
    case 54:
        //  TODO:BP模式，暂不可用
        bpArea = gui->getBPArea();
        bpArea->ban(arg[1].toInt(), arg[2].toInt());
        break;
    case 57:
        //  TODO:BP模式，暂不可用
        bpArea = gui->getBPArea();
        bpArea->choose(arg[1].toInt(), arg[2].toInt());
        decisionArea = gui->getDecisionArea();
        if(bpArea->checkOver())
        {
            bpArea->setVisible(0);
            disconnect(decisionArea,SIGNAL(okClicked()),this,SLOT(onOkClicked()));
            disconnect(bpArea,SIGNAL(roleReady()),this,SLOT(roleAnalyse()));
        }
        break;
    case 59:
        //  TODO:BP模式，暂不可用
        if(gui!=NULL)
            gui->chatAppend(arg[1].toInt(),arg[2]);
        break;
    }
}

void Logic::delete_proto(uint16_t type, google::protobuf::Message *proto)
{
    delete proto;
}

void Logic::onOkClicked()
{
    QStringList chosen;
    TipArea *tipArea;
    QList<int> roles;
    BPArea* bpArea;
    RoleItem* role;

    network::Pick* pick;
    switch(state)
    {
    case 46:
        tipArea=gui->getTipArea();
        chosen=tipArea->getBoxCurrentText().split(".");

        pick = new network::Pick();
        pick->set_role_id(chosen[0].toInt());

        emit sendCommand(network::MSG_PICK, pick);
        disconnect(gui->getDecisionArea(),SIGNAL(okClicked()),this,SLOT(onOkClicked()));;
        gui->reset();
        break;
    /*
    TODO:BP
    case 52:
        bpArea=gui->getBPArea();
        roles = bpArea->getSelectedRoles();
        emit sendCommand("53;"+QString::number(roles[0])+";");
        role= bpArea->getRoleByID(roles[0]);
        role->setY(role->y()+20);
        bpArea->reset();
        gui->reset();
        break;
    case 55:
        bpArea=gui->getBPArea();
        roles = bpArea->getSelectedRoles();
        emit sendCommand("56;"+QString::number(roles[0])+";");
        role= bpArea->getRoleByID(roles[0]);
        role->setY(role->y()+20);
        bpArea->reset();
        gui->reset();
        break;
    */
    }


}

void Logic::roleAnalyse()
{
    DecisionArea* decisionArea = gui->getDecisionArea();
    switch(state)
    {
    case 52:
    case 55:
        decisionArea->enable(0);
    }
}
