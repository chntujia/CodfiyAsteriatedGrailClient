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
#include "NvWuShen.h"
#include "YingLingRenXing.h"
#include "MoNv.h"
#include "ShiRen.h"
#include <QSound>
#include "Common.h"
#define NORMAL 0
#define READY 1
#define JOIN_TEAM 2

Logic* logic=NULL;
Logic::Logic(QObject *parent) :
    QObject(parent)
{
    count=0;
    hasShownRole=false;
    hasSetRole=false;
    myRole = NULL;
    state = -1;
    memset(roles,-1,sizeof(roles));
}

void Logic::setupRoom(bool isStarted, GameInfo *gameInfo)
{    
    if(!init_before_start && !isStarted) {
        connect(gui->getDecisionArea(),SIGNAL(okClicked()),this,SLOT(onOkClicked()));
        dataInterface->setupRoom(isStarted);
        gui->setupRoom(isStarted);
        normal();
        init_before_start = true;
    }
    else if(!init_after_start && isStarted){
        dataInterface->initPlayerList(gameInfo);
        dataInterface->setupRoom(isStarted);
        gui->setupRoom(isStarted);
        init_after_start = true;
        if(myID != GUEST){
            network::ReadyForGameRequest* ready = new ReadyForGameRequest;
            ready->set_type(ReadyForGameRequest_Type_SEAT_READY);
            emit sendCommand(network::MSG_READY_GAME_REQ, ready);
        }
    }
}

void Logic::cleanRoom()
{
    init_before_start = false;
    init_after_start = false;
    count = 0;
    dataInterface->cleanRoom();
    gui->cleanRoom();    
}

void Logic::destroyRoom(){
    network::LeaveRoomRequest* leave = new network::LeaveRoomRequest();
    socket->sendMessage(network::MSG_LEAVE_ROOM_REQ, leave);
    disconnect(socket, 0, this, 0);
    makeConnection();
    hasSetRole = false;
    if(myRole){
        delete myRole;
        myRole = NULL;
    }
}

void Logic::makeConnection()
{
    connect(socket, SIGNAL(getMessage(unsigned short, google::protobuf::Message*)),
            logic, SLOT(getCommand(unsigned short, google::protobuf::Message*)),
            Qt::QueuedConnection);
}

void Logic::setMyRole(int roleID)
{
    if(hasSetRole)
        return;
    switch(roleID)
    {
    case 1:
        myRole = new JianSheng;
        hasSetRole = true;
        break;
    case 2:
        myRole = new KuangZhan;
        hasSetRole = true;
        break;
    case 3:
        myRole = new GongNv;
        hasSetRole = true;
        break;
    case 4:
        myRole = new FengYin;
        hasSetRole = true;
        break;
    case 5:
        myRole = new AnSha;
        hasSetRole = true;
        break;
    case 6:
        myRole = new ShengNv;
        hasSetRole = true;
        break;
    case 7:
        myRole = new TianShi;
        hasSetRole = true;
        break;
    case 8:
        myRole = new MoDao;
        hasSetRole = true;
        break;
    case 9:
        myRole = new MoJian;
        hasSetRole = true;
        break;
    case 10:
        myRole = new ShengQiang;
        hasSetRole = true;
        break;
    case 11:
        myRole = new YuanSu;
        hasSetRole = true;
        break;
    case 12:
        myRole = new MaoXian;
        hasSetRole = true;
        break;
    case 13:
        myRole = new SiLing;
        hasSetRole = true;
        break;
    case 14:
        myRole = new ZhongCai;
        hasSetRole = true;
        break;
    case 15:
        myRole = new ShenGuan;
        hasSetRole = true;
        break;
    case 16:
        myRole = new QiDao;
        hasSetRole = true;
        break;
    case 17:
        myRole = new XianZhe;
        hasSetRole = true;
        break;
    case 18:
        myRole = new LingFu;
        hasSetRole = true;
        break;
    case 19:
        myRole = new JianDi;
        hasSetRole = true;
        break;
    case 20:
        myRole = new GeDouJia;
        hasSetRole = true;
        break;
    case 21:
        myRole = new YongZhe;
        hasSetRole = true;
        break;
    case 22:
        myRole = new LingHun;
        hasSetRole = true;
        break;
    case 23:
        myRole = new WuNv;
        hasSetRole = true;
        break;
    case 24:
        myRole = new DieWu;
        hasSetRole = true;
        break;
    case 25:
        myRole = new NvWuShen;
        hasSetRole = true;
        break;
    case 26:
        myRole = new MoGong;
        hasSetRole = true;
        break;
    case 27:
        myRole = new YingLingRenXing;
        hasSetRole = true;
        break;
    case 28:
        myRole = new HongLian;
        hasSetRole = true;
        break;
    case 29:
        myRole = new MoQiang;
        hasSetRole = true;
        break;
    case 30:
        myRole = new MoNv;
        hasSetRole = true;
        break;
    case 31:
        myRole = new ShiRen;
        hasSetRole = true;
        break;
    }

}

void Logic::getCommand(unsigned short proto_type, google::protobuf::Message* proto)
{
    QMutexLocker locker(&mutex);
    TipArea *tipArea;
    DecisionArea* decisionArea;
    BPArea* bpArea;
    SafeList<int> roleIDs;
    PlayerArea* playerArea;
    int targetID,roleID,howMany,num;
    QString arg[10];

    network::RoleRequest* char_pick;

    switch (proto_type)
    {
    case network::MSG_ROOMLIST_REP:
        lobby->fill((network::RoomListResponse*) proto);
        break;

    case network::MSG_GAME:
    {
        network::GameInfo* game_info = (network::GameInfo*) proto;
        if(game_info->has_player_id()){
            myID = game_info->player_id();
            dataInterface->setID(myID);            
            cleanRoom();
            if(myID == GUEST)
                gui->logAppend(QStringLiteral("<font color=\'pink\'>房间已满，进入观战模式</font>"));
        }
        if(game_info->has_room_id()){
            gui->getTeamArea()->setRoomID(game_info->room_id());
        }
        setupRoom(game_info->is_started(), game_info);

        for (int i = 0; i < game_info->player_infos_size(); ++i)
        {
            network::SinglePlayerInfo* player_info = (network::SinglePlayerInfo*)&(game_info->player_infos(i));
            targetID = player_info->id();
            if(player_info->has_role_id())
            {
                roleID = player_info->role_id();
                roles[targetID] = roleID;
                count++;
            }
            if(player_info->has_nickname()){
                dataInterface->setNickName(targetID, QString::fromStdString(player_info->nickname()));
            }
            if(player_info->has_team()){
                dataInterface->getPlayerById(targetID)->setTeam(player_info->team());
            }
            gui->getPlayerArea()->getPlayerItem(targetID)->setReady(player_info->ready());
        }

        if(count==dataInterface->getPlayerMax())
        {
            disconnect(getClient(),0,this,0);
            disconnect(gui->getDecisionArea(), 0, this, 0);
            for(int i=0;i<dataInterface->getPlayerMax();i++){
                dataInterface->getPlayerList().at(i)->setRole(roles[i]);
                gui->getPlayerArea()->getPlayerItem(i)->setToolTip(dataInterface->getRoleSkillInfo(roles[i]));
            }

            roleID = myID == GUEST ? roles[0] : roles[myID];
            setMyRole(roleID);
            network::GameInfo* toRole = new network::GameInfo;
            toRole->CopyFrom(*game_info);
            myRole->decipher(network::MSG_GAME, toRole);
        }
        gui->getPlayerArea()->update();
        break;
    }
    case network::MSG_GOSSIP:
        if(gui!=NULL)
        {
            network::Gossip* gossip = (network::Gossip*) proto;
            gui->chatAppend(gossip->id(), QString::fromStdString(gossip->txt()));
        }
        break;
    case network::MSG_ERROR:
    {
        network::Error* error = (network::Error*)proto;
        if(error->dst_id() == myID){
            gui->reset();
        }
        switch(error->id())
        {
        //GE_DISCONNECTED
        case GE_DISCONNECTED:
            gui->logAppend(QStringLiteral("<font color=\'red\'>玩家") + QString::number(error->dst_id()) + QStringLiteral("掉线</font>"));
            break;
        case GE_NOT_WELCOME:
            gui->logAppend(QStringLiteral("<font color=\'red\'>啊咧？您不受待见呢。。</font>"));
            break;
        case GE_WRONG_PASSWORD:
            gui->logAppend(QStringLiteral("<font color=\'red\'>暗语不对呢。。</font>"));
            break;
        default:
            gui->logAppend(QStringLiteral("<font color=\'red\'>错误代码") + QString::number(error->id()) + "</font>");
            break;
        }
        break;
    }
//选择角色
    case network::MSG_ROLE_REQ:
        state=46;
        char_pick = (network::RoleRequest*) proto;

        tipArea=gui->getTipArea();
        decisionArea=gui->getDecisionArea();
        tipArea->reset();        
        howMany=char_pick->role_ids_size();
        for(int i=0;i<howMany;i++){
            roleID=char_pick->role_ids(i);
            tipArea->addBoxItem(QString::number(roleID)+"."+dataInterface->getRoleName(roleID));
        }
        tipArea->setMsg(QStringLiteral("请选择角色："));
        tipArea->showBox();
        decisionArea->enable(0);
        gui->alert();
        QSound::play("sound/Warning.wav");
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
    delete proto;
}

void Logic::onOkClicked()
{
    QMutexLocker locker(&mutex);

    switch(state)
    {
    case JOIN_TEAM:
    {
        TipArea *tipArea = gui->getTipArea();
        int chosen = tipArea->getBoxCurrentText().split(".")[0].toInt();
        network::JoinTeamRequest *join = new network::JoinTeamRequest();
        join->set_team((network::JoinTeamRequest_Team)(chosen - 1));
        emit sendCommand(network::MSG_JOIN_TEAM_REQ, join);
        normal();
        break;
    }
    case 46:
    {
        network::PickBan* pick;
        TipArea *tipArea = gui->getTipArea();
        int chosen = tipArea->getBoxCurrentText().split(".")[0].toInt();

        pick = new network::PickBan();
        pick->add_role_ids(chosen);
        pick->set_strategy(network::ROLE_STRATEGY_31);
        pick->set_is_pick(true);

        emit sendCommand(network::MSG_PICK_BAN, pick);
        disconnect(gui->getDecisionArea(),SIGNAL(okClicked()),this,SLOT(onOkClicked()));
        gui->reset();
        break;
    }
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

void Logic::normal()
{
    state = NORMAL;
    gui->reset();
    if(myID != GUEST){
        ButtonArea* buttonArea = gui->getButtonArea();
        buttonArea->enable(1);
        buttonArea->enable(2);
    }
}

void Logic::ready()
{
    state = READY;
    gui->reset();
    ButtonArea* buttonArea = gui->getButtonArea();
    buttonArea->enable(1);
    Button* button = buttonArea->getButtons().at(1);
    button->setSelected(true);

    network::ReadyForGameRequest* ready = new ReadyForGameRequest;
    ready->set_type(ReadyForGameRequest_Type_START_READY);
    emit sendCommand(network::MSG_READY_GAME_REQ, ready);
}

void Logic::joinTeam()
{
    state = JOIN_TEAM;
    TipArea *tipArea = gui->getTipArea();
    DecisionArea *decisionArea = gui->getDecisionArea();
    tipArea->addBoxItem(QStringLiteral("1.蓝B"));
    tipArea->addBoxItem(QStringLiteral("2.红A"));
    tipArea->addBoxItem(QStringLiteral("3.随机"));
    tipArea->showBox();
    tipArea->setMsg(QStringLiteral("请选择"));
    decisionArea->enable(0);
}

void Logic::onButtonClicked(int id)
{
    switch(id)
    {
    //点名
    case 0:
        break;
    //准备
    case READY:
        ready();
        break;
    //选队
    case JOIN_TEAM:
        joinTeam();
        break;
    }
}

void Logic::onButtonUnclicked(int id)
{
    switch(id)
    {
    //准备
    case READY:
    {
        normal();
        network::ReadyForGameRequest* ready = new ReadyForGameRequest;
        ready->set_type(ReadyForGameRequest_Type_CANCEL_START_REDAY);
        emit sendCommand(network::MSG_READY_GAME_REQ, ready);
        break;
    }
    default:
        normal();
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

void Logic::onError(int error)
{
    TipArea* tipArea;
    if(gui && (tipArea = gui->getTipArea())){
        tipArea->setMsg(QStringLiteral("错误代码：") + QString::number(error)
                  + QStringLiteral(";可尝试等待系统重发"));
    }
}
