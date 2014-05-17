#ifndef ROLE_H
#define ROLE_H

#include <QObject>
#include "widget/GUI.h"
#include "data/DataInterface.h"
#include "Logic.h"

class Role : public QObject
{
    Q_OBJECT
public:
    explicit Role(QObject *parent = 0);
    virtual void turnBegin();
    virtual void normal();
    virtual void makeConnection();
    virtual void attackAction();
    virtual void magicAction();
    virtual void attackOrMagic();
    virtual void additionalAction(Command *cmd);
    virtual void attacked(QString element,int hitRate);
    virtual void moDaned(int nextID, int sourceID, int howMany);
    virtual void cure(int cross,int harmPoint,int type, int crossAvailable);
    virtual void askForSkill(network::Command* cmd);
    void setAttackTarget();
    virtual void drop(int howMany, int cause);
    virtual void dropCover(int howMany, int cause);
    void unactionalCheck();
    network::Action* newAction(uint32_t action_type);
    network::Action* newAction(uint32_t action_type, uint32_t action_id);
    network::Respond* newRespond(uint32_t respond_id);

    void JiAngStone();
    void JiAngHarm();

    void ShengLiStone();
signals:
    void sendCommand(uint16_t proto_type, google::protobuf::Message* proto);

public slots:
    virtual void decipher(uint16_t, google::protobuf::Message*);
    virtual void onOkClicked();
    virtual void onCancelClicked();
    virtual void cardAnalyse();
    virtual void coverCardAnalyse();
    virtual void playerAnalyse();
    void exchangeCards();
    void resign();
    void buy();
    void synthetize();
    void extract();
    void setMyRole(Role* role){myRole=role;}
protected:
    int myID;
    int state;
    int sourceID;
    int moDanNextID;
    int nextCounterClockwise;
    bool isMyTurn;
    bool onceUsed;
    int actionFlag;
    SafeList<Player*>playerList;
    HandArea*handArea;
    HandArea *coverArea;
    PlayerArea*playerArea;
    ButtonArea*buttonArea;
    DecisionArea*decisionArea;
    TipArea*tipArea;
    TeamArea*teamArea;
    ShowArea* showArea;
    bool usedAttack,usedMagic,usedSpecial,start;
    Role* myRole;
    Team*blue,*red;
    int chosenAction;
	//FIXME: 暂时只保存了额外行动跟响应类技能时的CMD
    network::Command skillCmd;
    QMutex mutex;

    int ShiRenChoice;
};



#endif // ROLE_H
