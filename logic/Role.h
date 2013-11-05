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
    void drop(int howMany);
    void dropCover(int howMany);
    void unactionalCheck();
    void TianShiZhuFu(int n);
    void MoBaoChongJi();
    void ChongYing(int color);
    network::Action* newAction(uint32_t action_id);
    network::Respond* newRespond(uint32_t respond_id);
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
    QList<Player*>playerList;
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
    google::protobuf::Message* proto;
    network::Command* cmd;
};



#endif // ROLE_H
