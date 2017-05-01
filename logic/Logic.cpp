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
#include "spMoDao.h"
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
#ifdef SOUND
  #include <QSound>
#endif
#include "Common.h"
#define NORMAL 3
#define READY 0
#define JOIN_TEAM 1
#define POLLING 61

Logic* logic=NULL;
Logic::Logic(QObject *parent) :
    QObject(parent)
{
    hasShownRole = false;
    myRole = NULL;
    state = -1;
    memset(roles,-1,sizeof(roles));
}

void Logic::setupRoom(bool isStarted, GameInfo *gameInfo)
{
    if(!init_before_start && !isStarted) {
        connect(gui->getDecisionArea(),SIGNAL(okClicked()),this,SLOT(onOkClicked()));
        connect(gui->getDecisionArea(),SIGNAL(cancelClicked()),this,SLOT(onCancelClicked()));
        connect(gui->getBPArea(),SIGNAL(roleReady()),this,SLOT(roleAnalyse()));
        dataInterface->setupRoom(isStarted);
        gui->setupRoom(isStarted);
        normal();
        init_before_start = true;
#ifdef DEBUG
        ready();
#endif
    }
    else if(!init_after_start && isStarted){
        dataInterface->initPlayerList(gameInfo);
        dataInterface->setupRoom(isStarted);
        gui->setupRoom(isStarted);
        init_after_start = true;
    }
}

void Logic::cleanRoom()
{
    init_before_start = false;
    init_after_start = false;
    muteList.clear();
    dataInterface->cleanRoom();
    gui->cleanRoom();
}

void Logic::destroyRoom(){
    network::LeaveRoomRequest* leave = new network::LeaveRoomRequest();
    socket->sendMessage(network::MSG_LEAVE_ROOM_REQ, leave);
    disconnect(socket, 0, this, 0);
    makeConnection();
    if(myRole){
        delete myRole;
        myRole = NULL;
    }
}

void Logic::makeConnection()
{
    connect(socket, SIGNAL(getMessage(unsigned short, google::protobuf::Message*)),
            logic, SLOT(getCommand(unsigned short, google::protobuf::Message*)));
}

void Logic::setMyRole(int roleID)
{
    if(myRole)
        return;
    switch(roleID)
    {
    case 1:
        myRole = new JianSheng;      
        break;
    case 2:
        myRole = new KuangZhan;
        break;
    case 3:
        myRole = new GongNv;
        break;
    case 4:
        myRole = new FengYin;
        break;
    case 5:
        myRole = new AnSha;
        break;
    case 6:
        myRole = new ShengNv;
        break;
    case 7:
        myRole = new TianShi;
        break;
    case 8:
        myRole = new MoDao;
        break;
    case 9:
        myRole = new MoJian;
        break;
    case 10:
        myRole = new ShengQiang;
        break;
    case 11:
        myRole = new YuanSu;
        break;
    case 12:
        myRole = new MaoXian;
        break;
    case 13:
        myRole = new SiLing;
        break;
    case 14:
        myRole = new ZhongCai;
        break;
    case 15:
        myRole = new ShenGuan;
        break;
    case 16:
        myRole = new QiDao;
        break;
    case 17:
        myRole = new XianZhe;
        break;
    case 18:
        myRole = new LingFu;
        break;
    case 19:
        myRole = new JianDi;
        break;
    case 20:
        myRole = new GeDouJia;
        break;
    case 21:
        myRole = new YongZhe;
        break;
    case 22:
        myRole = new LingHun;
        break;
    case 23:
        myRole = new WuNv;
        break;
    case 24:
        myRole = new DieWu;
        break;
    case 25:
        myRole = new NvWuShen;
        break;
    case 26:
        myRole = new MoGong;
        break;
    case 27:
        myRole = new YingLingRenXing;
        break;
    case 28:
        myRole = new HongLian;
        break;
    case 29:
        myRole = new MoQiang;
        break;
    case 30:
        myRole = new MoNv;
        break;
    case 31:
        myRole = new ShiRen;
        break;
    case 108:
        myRole = new spMoDao;
        break;
    }

}

void Logic::getCommand(unsigned short proto_type, google::protobuf::Message* proto)
{
    TipArea *tipArea;
    DecisionArea* decisionArea;
    BPArea* bpArea;
    SafeList<int> roleIDs;
    SafeList<int> options;
    PlayerArea* playerArea;
    int targetID,roleID,howMany;    

    network::RoleRequest* char_pick;
    int targetId;
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
            else{
                gui->logAppend(QStringLiteral("<font color=\'pink\'>请准备</font>"));
                gui->logAppend(QStringLiteral("<font color=\'pink\'>觉得某人烦的话，可以“mute n”，n是该玩家座次；“unmute n”恢复</font>"));
            }
        }
        if(game_info->has_room_id()){
            gui->getTeamArea()->setRoomID(game_info->room_id());
        }
        int count = 0;
        for (int i = 0; i < game_info->player_infos_size(); ++i)
        {
            network::SinglePlayerInfo* player_info = (network::SinglePlayerInfo*)&(game_info->player_infos(i));
            if(player_info->has_role_id())
                count++;
        }

        setupRoom(count == dataInterface->getPlayerMax(), game_info);

        for (int i = 0; i < game_info->player_infos_size(); ++i)
        {
            network::SinglePlayerInfo* player_info = (network::SinglePlayerInfo*)&(game_info->player_infos(i));
            targetID = player_info->id();
            if(player_info->has_role_id())
            {
                roleID = player_info->role_id();
                roles[targetID] = roleID;
                dataInterface->getPlayerList().at(targetID)->setRole(roleID);
                gui->getPlayerArea()->getPlayerItem(targetID)->setToolTip(dataInterface->getRoleSkillInfo(roleID));
            }
            if(player_info->has_nickname()){
                dataInterface->setNickName(targetID, QString::fromStdString(player_info->nickname()));
            }
            if(player_info->has_team()){
                dataInterface->getPlayerById(targetID)->setTeam(player_info->team());
            }
            gui->getPlayerArea()->getPlayerItem(targetID)->setReady(player_info->ready());
            if(player_info->has_leader()){
                gui->getPlayerArea()->getPlayerItem(targetID)->setLeader(player_info->leader());
            }
        }


        if(count == dataInterface->getPlayerMax())
        {
            disconnect(getClient(),0,this,0);
            disconnect(gui->getDecisionArea(), 0, this, 0);
            disconnect(gui->getBPArea(),0,this,0);

            roleID = myID == GUEST ? roles[0] : roles[myID];
            setMyRole(roleID);
            network::GameInfo* toRole = new network::GameInfo;
            toRole->CopyFrom(*game_info);
            myRole->decipher(network::MSG_GAME, toRole);
            gui->hideBP();
        }
        gui->getPlayerArea()->update();
        break;
    }

    case network::MSG_BECOME_LEADER_REQ:
        state = 60;
        gui->reset();
        tipArea = gui->getTipArea();
        tipArea->setMsg(QStringLiteral("要申请做队长吗？"));

        decisionArea = gui->getDecisionArea();
        decisionArea->enable(0);
        decisionArea->enable(1);
        gui->alert();
        break;

    case network::MSG_GOSSIP:
        if(gui!=NULL)
        {
            network::Gossip* gossip = (network::Gossip*)proto;
            switch (gossip->type())
            {
            case network::GOSSIP_TALK:
                gui->chatAppend(gossip->id(), QString(gossip->txt().c_str()));
                break;
            case network::GOSSIP_NOTICE:
                gui->logAppend(QString(gossip->txt().c_str()));
                break;
            }
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
        case GE_DISCONNECTED:
        {
            int targetId = error->dst_id();
            gui->logAppend(QStringLiteral("<font color=\'red\'>玩家") + QString::number(targetId) + QStringLiteral("离开房间</font>"));
            dataInterface->setNickName(targetId, "");
            dataInterface->getPlayerById(targetId)->setTeam(-1);
            gui->getPlayerArea()->getPlayerItem(targetId)->setReady(false);
            gui->getPlayerArea()->update();
            break;
        }
        case GE_NOT_WELCOME:
            gui->logAppend(QStringLiteral("<font color=\'red\'>这个房主比较高冷，下个房间见。。</font>"));
            break;
        case GE_WRONG_PASSWORD:
            gui->logAppend(QStringLiteral("<font color=\'red\'>瞎蒙果然是不行的。。</font>"));
            break;
        default:
            gui->logAppend(QStringLiteral("<font color=\'red\'>错误代码") + QString::number(error->id()) + "</font>");
            break;
            }
        break;
    }
//选择角色
    case network::MSG_ROLE_REQ:
    {
        char_pick = (network::RoleRequest*) proto;
        targetId = char_pick->id();
        if(char_pick->strategy() == ROLE_STRATEGY_31)
        {
            gui->logAppend(QStringLiteral("<font color=\'white\'>等待玩家") + QString::number(targetId) + QStringLiteral("选择角色")+"</front");
            if(targetId != myID)
                break;
            state=46;
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
            break;
        }
        else if(char_pick->strategy() == ROLE_STRATEGY_BP)
        {
            state = 51;
            tipArea=gui->getTipArea();
            decisionArea=gui->getDecisionArea();
            bpArea = gui->getBPArea();
            tipArea->reset();
            howMany=char_pick->role_ids_size();
            for(int i=0;i<howMany;i++){
                roleIDs << char_pick->role_ids(i);
                options << char_pick->args(i);
            }
            bpArea->BPStart(howMany, roleIDs, options, char_pick->opration());
            if(char_pick->opration() ==  BP_NULL )
            {
                int lastChosen = -1;
                int step = 0;
                for(int i = 0; i < howMany; i++){
                    if(options[i] > step){
                        step = options[i];
                        lastChosen = i;
                    }
                }
                if(step == 0){
                    gui->logAppend(QStringLiteral("以下英雄响应了召唤："));
                    QString msg;
                    foreach(int id, roleIDs)
                        msg += QStringLiteral("<font color=\'yellow\'>%1 </font>").arg(dataInterface->getRoleName(id));
                    gui->logAppend(msg);
                }
                else {
                    QString msg = QStringLiteral("<font color=\'yellow\'>%1</font> %2 了<font color=\'yellow\'>%3</font>");
                    QString nickname = dataInterface->getPlayerList().at(targetId)->getNickname();
                    gui->logAppend(msg.arg(nickname, ((options[lastChosen]-1) % 4) < 2 ? "BAN" : "PICK", dataInterface->getRoleName(roleIDs[lastChosen])));
                }
            }
            else
            {
                QString msg = QStringLiteral("等待<font color=\'yellow\'> %1</font> %2 角色");
                QString nickname = dataInterface->getPlayerList().at(targetId)->getNickname();
                gui->logAppend(msg.arg(nickname, char_pick->opration()==BP_PICK ? "PICK" : "BAN"));
            }
            if(targetId != myID)
                break;
            if(char_pick->opration() == BP_BAN )
            {
                state = 52;
                bpArea = gui->getBPArea();
                bpArea->setMsg(QStringLiteral("请禁用一角色"));
                bpArea->setQuota(1);
                bpArea->enableAll();
                playerArea = gui->getPlayerArea();
                gui->alert();
            }
            else if(char_pick->opration() == BP_PICK )
            {
                state = 53;
                bpArea = gui->getBPArea();
                bpArea->setMsg(QStringLiteral("请选择一角色"));
                bpArea->setQuota(1);
                bpArea->enableAll();
                playerArea = gui->getPlayerArea();
                gui->alert();
            }
            break;
        }
        else if(char_pick->strategy() == ROLE_STRATEGY_CM)
        {
            state = 51;
            tipArea=gui->getTipArea();
            decisionArea=gui->getDecisionArea();
            bpArea = gui->getBPArea();
            bpArea->show();
            tipArea->reset();
            howMany=char_pick->role_ids_size();
            int lastChosen = -1;
            for(int i=0;i<howMany;i++){
                roleIDs << char_pick->role_ids(i);
                if(char_pick->args(i)>0){
                    if(lastChosen==-1)
                    lastChosen =0;
                    if(char_pick->args(i)>10){
                        lastChosen =i;
                        options << (char_pick->args(i) - 10);
                        continue;
                    }
                }
                    options << char_pick->args(i);
            }

            QString oprating;
            if(char_pick->opration() ==  CM_NULL)
            {
            if (lastChosen >0)
            {
                switch(options[lastChosen])
                {
                case CM_RED_BAN:
                case CM_BLUE_BAN:
                    oprating = QStringLiteral("禁用");break;
                case CM_RED_PICK:
                case CM_BLUE_PICK:
                    oprating = QStringLiteral("选择");break;
                case CM_RED_IB:
                case CM_BLUE_IB:
                    oprating = QStringLiteral("额外禁用");break;
                default:
                    oprating = QStringLiteral("错误");break;
                }
            }
            else if(lastChosen==-1)
                oprating = QStringLiteral("禁用");
            else if(lastChosen==0)
                oprating = QStringLiteral("没有额外禁用");
            }
            else
            {
                switch(char_pick->opration())
                {
                case CM_RED_BAN:
                case CM_BLUE_BAN:
                    oprating = QStringLiteral("禁用");break;
                case CM_RED_PICK:
                case CM_BLUE_PICK:
                    oprating = QStringLiteral("选择");break;
                case CM_RED_IB:
                case CM_BLUE_IB:
                    oprating = QStringLiteral("额外禁用");break;
                default:
                    oprating = QStringLiteral("错误");break;
                }
            }
            bpArea->CMStart(howMany, roleIDs, options, char_pick->opration());
            if(char_pick->opration() ==  CM_NULL)
            {
                if(lastChosen == -1){
                    gui->logAppend(QStringLiteral("以下英雄响应了召唤："));
                    QString msg;
                    foreach(int id, roleIDs)
                        msg += QStringLiteral("<font color=\'yellow\'>%1 </font>").arg(dataInterface->getRoleName(id));
                    gui->logAppend(msg);
                }
                else {
                    QString msg = QStringLiteral("<font color=\'yellow\'>%1 </font>%2 <font color=\'yellow\'>%3</font>");
                    QString nickname = dataInterface->getPlayerList().at(targetId)->getNickname();
                    if(lastChosen==0)gui->logAppend(msg.arg(nickname, oprating, dataInterface->getRoleName(0)));
                    else
                    gui->logAppend(msg.arg(nickname, oprating+QStringLiteral("了"),dataInterface->getRoleName(roleIDs[lastChosen])));
                }
            }
            else
            {
                QString msg = QStringLiteral("等待<font color=\'yellow\'> %1</font> %2 角色");
                QString nickname = dataInterface->getPlayerList().at(targetId)->getNickname();
                gui->logAppend(msg.arg(nickname, oprating));
            }
            if(targetId != myID)
                break;
            switch(char_pick->opration())
            {
            case CM_RED_BAN:
            {
                state = 54;
                bpArea = gui->getBPArea();
                bpArea->setMsg(QStringLiteral("请禁用一角色"));
                bpArea->setQuota(1);
                bpArea->enableAll();
                playerArea = gui->getPlayerArea();
                gui->alert();
                break;
            }
            case CM_BLUE_BAN:
            {
                state = 55;
                bpArea = gui->getBPArea();
                bpArea->setMsg(QStringLiteral("请禁用一角色"));
                bpArea->setQuota(1);
                bpArea->enableAll();
                playerArea = gui->getPlayerArea();
                gui->alert();
                break;
            }
            case CM_RED_PICK:
            {
                state = 56;
                bpArea = gui->getBPArea();
                bpArea->setMsg(QStringLiteral("请选择一角色"));
                bpArea->setQuota(1);
                bpArea->enableAll();
                playerArea = gui->getPlayerArea();
                gui->alert();
                break;
            }
            case CM_BLUE_PICK:
            {
                state = 57;
                bpArea = gui->getBPArea();
                bpArea->setMsg(QStringLiteral("请选择一角色"));
                bpArea->setQuota(1);
                bpArea->enableAll();
                playerArea = gui->getPlayerArea();
                gui->alert();
                break;
            }
            case CM_RED_IB:
            {
                state = 58;
                bpArea = gui->getBPArea();
                bpArea->setMsg(QStringLiteral("请额外禁用角色，或点‘取消’"));
                bpArea->setQuota(1);
                bpArea->enableAll();
                decisionArea = gui->getDecisionArea();
                decisionArea->enable(1);
                playerArea = gui->getPlayerArea();
                gui->alert();
                break;
            }
            case CM_BLUE_IB:
            {
                state = 59;
                bpArea = gui->getBPArea();
                bpArea->setMsg(QStringLiteral("请额外禁用角色，或点‘取消’"));
                bpArea->setQuota(1);
                bpArea->enableAll();
                playerArea = gui->getPlayerArea();
                decisionArea = gui->getDecisionArea();
                decisionArea->enable(1);
                gui->alert();
                break;
            }
            }

        }
        break;
    }
    case network::MSG_POLLING_REQ:
    {
        state = POLLING;
        network::PollingRequest* req = (network::PollingRequest*) proto;
        gui->hideBP();
        gui->reset();
        tipArea = gui->getTipArea();
        tipArea->setMsg(QString::fromStdString(req->object()));
        for(int i = 0; i < req->options_size(); i++){
            tipArea->addBoxItem(QString::fromStdString(req->options(i)));
        }
        tipArea->showBox();

        decisionArea = gui->getDecisionArea();
        decisionArea->enable(0);
        gui->alert();
        break;
    }
    }
    delete proto;
}

void Logic::onOkClicked()
{

    int chosen = 0;
    SafeList<int> selected;
    BPArea* bpArea = NULL;
    network::PickBan* pick;
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
        gui->reset();
        break;
    }
    case 52:
        bpArea=gui->getBPArea();
        selected = bpArea->getSelectedRoles();
        chosen = selected[0];
        pick = new network::PickBan();
        pick->add_role_ids(chosen);
        pick->set_strategy(network::ROLE_STRATEGY_BP);
        pick->set_is_pick(false);
        emit sendCommand(network::MSG_PICK_BAN, pick);
        bpArea->reset();
        gui->reset();
        break;
    case 53:
        bpArea=gui->getBPArea();
        selected = bpArea->getSelectedRoles();
        chosen = selected[0];
        pick = new network::PickBan();
        pick->add_role_ids(chosen);
        pick->set_strategy(network::ROLE_STRATEGY_BP);
        pick->set_is_pick(true);
        emit sendCommand(network::MSG_PICK_BAN, pick);
        bpArea->reset();
        gui->reset();
        break;
    case 54:
    case 55:
        bpArea=gui->getBPArea();
        selected = bpArea->getSelectedRoles();
        chosen = selected[0];
        pick = new network::PickBan();
        pick->add_role_ids(chosen);
        pick->set_strategy(network::ROLE_STRATEGY_CM);
        pick->set_is_pick(false);
        emit sendCommand(network::MSG_PICK_BAN, pick);
        bpArea->reset();
        gui->reset();
        break;
    case 56:
    case 57:
        bpArea=gui->getBPArea();
        selected = bpArea->getSelectedRoles();
        chosen = selected[0];
        pick = new network::PickBan();
        pick->add_role_ids(chosen);
        pick->set_strategy(network::ROLE_STRATEGY_CM);
        pick->set_is_pick(true);
        emit sendCommand(network::MSG_PICK_BAN, pick);
        bpArea->reset();
        gui->reset();
        break;
    case 58:
    case 59:
        bpArea=gui->getBPArea();
        selected = bpArea->getSelectedRoles();
        chosen = selected[0];
        pick = new network::PickBan();
        pick->add_role_ids(chosen);
        pick->set_strategy(network::ROLE_STRATEGY_CM);
        pick->set_is_pick(false);
        emit sendCommand(network::MSG_PICK_BAN, pick);
        bpArea->reset();
        gui->reset();
        break;
    case 60:
    {
        BecomeLeaderResponse *res = new BecomeLeaderResponse;
        res->set_yes(1);
        emit sendCommand(network::MSG_BECOME_LEADER_REP, res);
        gui->reset();
        break;
    }
    case POLLING:
    {
        PollingResponse *res = new PollingResponse;
        TipArea *tipArea = gui->getTipArea();
        int chosen = tipArea->getBoxCurrentIndex();
        res->set_option(chosen);
        emit sendCommand(network::MSG_BECOME_LEADER_REP, res);
        gui->reset();
        break;
    }
    }


}
void Logic::onCancelClicked()
{
    network::PickBan* pick;
    BPArea* bpArea = NULL;
    switch(state)
    {
    case 58:
    case 59:
        pick = new network::PickBan();
        pick->add_role_ids(100);
        pick->set_strategy(network::ROLE_STRATEGY_CM);
        pick->set_is_pick(false);
        emit sendCommand(network::MSG_PICK_BAN, pick);
        bpArea=gui->getBPArea();
        bpArea->reset();
        gui->reset();
        break;
    case 60:
        BecomeLeaderResponse *res = new BecomeLeaderResponse;
        res->set_yes(0);
        emit sendCommand(network::MSG_BECOME_LEADER_REP, res);
        gui->reset();
        break;

    }
}

void Logic::normal()
{
    state = NORMAL;
    gui->reset();
    if(myID != GUEST){
        ButtonArea* buttonArea = gui->getButtonArea();
        buttonArea->enable(0);
        buttonArea->enable(1);
    }
}

void Logic::ready()
{
    state = READY;
    gui->reset();
    ButtonArea* buttonArea = gui->getButtonArea();
    buttonArea->enable(0);
    Button* button = buttonArea->getButtons().at(0);
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
ACCOUNT_STATUS Logic::getIdentity() const
{
    return identity;
}

void Logic::setIdentity(const ACCOUNT_STATUS &value)
{
    identity = value;
}


void Logic::roleAnalyse()
{
    DecisionArea* decisionArea = gui->getDecisionArea();
    switch(state)
    {
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
    case 58:
    case 59:
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

void Logic::muteUser(int userId)
{
    muteList += userId;
}

void Logic::unmuteUser(int userId)
{
    muteList -= userId;
}

bool Logic::isMuted(int userId)
{
    return muteList.contains((userId));
}
