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
    state = -1;
    memset(roles,-1,sizeof(roles));
}
void Logic::readyToStart()
{
    emit sendCommand("16;");
}
void Logic::setMyRole(int roleID)
{
    switch(roleID)
    {
    case 1:
        new JianSheng;
        break;
    case 2:
        new KuangZhan;
        break;
    case 3:
        new GongNv;
        break;
    case 4:
        new FengYin;
        break;
    case 5:
        new AnSha;
        break;
    case 6:
        new ShengNv;
        break;
    case 7:
        new TianShi;
        break;
    case 8:
        new MoDao;
        break;
    case 9:
        new MoJian;
        break;
    case 10:
        new ShengQiang;
        break;
    case 11:
        new YuanSu;
        break;
    case 12:
        new MaoXian;
        break;
    case 13:
        new SiLing;
        break;
    case 14:
        new ZhongCai;
        break;
    case 15:
        new ShenGuan;
        break;
    case 16:
        new QiDao;
        break;
    case 17:
        new XianZhe;
        break;
    case 18:
        new LingFu;
        break;
    case 19:
        new JianDi;
        break;
    case 20:
        new GeDouJia;
        break;
    case 21:
        new YongZhe;
        break;
    case 22:
        new LingHun;
        break;
    case 23:
        new WuNv;
        break;
    case 24:
        new DieWu;
        break;
    case 26:
        new MoGong;
        break;
    case 28:
        new HongLian;
        break;
    case 29:
        new MoQiang;
        break;
    }
}

void Logic::getCommand(QString command)
{
    QStringList arg=command.split(';');
    TipArea *tipArea;
    DecisionArea* decisionArea;
    BPArea* bpArea;
    QList<int> roleIDs;
    PlayerArea* playerArea;
    QStringList nicknames;
    int playerMax,targetID,roleID,howMany,num;
    QString temp;

    switch (arg[0].toInt())
    {
    //Port Redirect
    case 150:
        temp=socket->GetAddr();
        socket->disconnectFromHost();
        socket->connectToHost(temp, arg[1].toInt());
        socket->forwarded=true;
        emit sendCommand("0;"+QString::number(socket->isRed)+";"+socket->nickname+";");
        break;
    case LOGINPERMIT:
        myID=arg[1].toInt();
        dataInterface->setID(myID);
        break;
    case GAMESTART:
        if(state == -1){
            state = GAMESTART;
            playerMax=arg[1].count()/2;
            dataInterface->setPlayerMax(playerMax);
            //nicknames=arg[2].split(',');
            dataInterface->initPlayerList(arg[1],nicknames);
            if (playerMax==8)
                dataInterface->initTeam(18);
            else
                dataInterface->initTeam(15);
        }
//dataInterface初始化完成
        emit gameStart();
        break;
//角色宣告
    case 37:
        targetID=arg[1].toInt();
        roleID=arg[2].toInt();
        roles[targetID]=roleID;
        if(arg[3]=="1"){
            dataInterface->getPlayerList().at(targetID)->setRole(roleID);
            gui->getPlayerArea()->update();
            hasShownRole=true;
        }
        if(targetID==myID){
            dataInterface->getPlayerList().at(targetID)->setRole(roleID);
            setMyRole(roleID);
            gui->getPlayerArea()->getPlayerItem(targetID)->setToolTip(dataInterface->getRoleSkillInfo(roleID));
            gui->getPlayerArea()->update();
        }
        count++;
        if(count==dataInterface->getPlayerMax())
        {
            disconnect(getClient(),0,this,0);
            if(!hasShownRole){
                for(int i=0;i<dataInterface->getPlayerMax();i++)
                    dataInterface->getPlayerList().at(i)->setRole(roles[i]);
                gui->getPlayerArea()->update();
            }

            for(int i=0;i<dataInterface->getPlayerMax();i++){
                gui->getPlayerArea()->getPlayerItem(i)->setToolTip(dataInterface->getRoleSkillInfo(roles[i]));
            }
        }
        break;
    case 38:
        if(gui!=NULL)
            gui->logAppend(arg[1]);
        break;
//31选择角色
    case 46:
        state=46;
        tipArea=gui->getTipArea();
        decisionArea=gui->getDecisionArea();
        tipArea->reset();
        connect(decisionArea,SIGNAL(okClicked()),this,SLOT(onOkClicked()));
        howMany=arg[1].toInt();
        for(int i=0;i<howMany;i++){
            roleID=arg[2+i].toInt();
            tipArea->addBoxItem(QString::number(roleID)+"."+dataInterface->getRoleName(roleID));
        }
        tipArea->setMsg(QStringLiteral("请选择角色："));
        tipArea->showBox();
        decisionArea->enable(0);
        break;
    case 51:
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
        state = 52;
        bpArea = gui->getBPArea();
        bpArea->setMsg("请禁用一角色");
        bpArea->setQuota(1);
        bpArea->enableAll();
        playerArea = gui->getPlayerArea();
        gui->alert();
        break;
    case 55:
        state = 55;
        bpArea = gui->getBPArea();
        bpArea->setMsg("请选择一角色");
        bpArea->setQuota(1);
        bpArea->enableAll();
        playerArea = gui->getPlayerArea();
        gui->alert();
        break;
    case 54:
        bpArea = gui->getBPArea();
        bpArea->ban(arg[1].toInt(), arg[2].toInt());
        break;
    case 57:
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
        if(gui!=NULL)
            gui->chatAppend(arg[1].toInt(),arg[2]);
        break;
    }
}
void Logic::onOkClicked()
{
    QStringList chosen;
    TipArea *tipArea;
    QList<int> roles;
    BPArea* bpArea;
    RoleItem* role;
    switch(state)
    {
    case 46:
        tipArea=gui->getTipArea();
        chosen=tipArea->getBoxCurrentText().split(".");
        emit sendCommand("47;"+chosen[0]+";");
        disconnect(gui->getDecisionArea(),SIGNAL(okClicked()),this,SLOT(onOkClicked()));;
        gui->reset();
        break;
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
