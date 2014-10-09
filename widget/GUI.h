#ifndef GUI_H
#define GUI_H

#include <QObject>
#include <QApplication>
#include "HandArea.h"
#include "PlayerArea.h"
#include "ShowArea.h"
#include "TeamArea.h"
#include "TipArea.h"
#include "ButtonArea.h"
#include "LogArea.h"
#include "BPArea.h"
#include "Animation.h"
#include "Timelinebar.h"

class GUI : public QObject
{
    Q_OBJECT
public:
    explicit GUI(QObject *parent = 0);
    void setupRoom(bool isStarted);
    void cleanRoom();
    QWidget* newWindow(int playerNum);
    void setWindow(QWidget*win){window=win;}
    void setHandArea(HandArea* handArea);
    void setPlayerArea(PlayerArea* playerArea);
    void setShowArea(ShowArea* showArea);
    void setTeamArea(TeamArea* teamArea);
    void setTipArea(TipArea* tipArea);
    void setDecisionArea(DecisionArea*decisionArea);
    void setButtonArea(ButtonArea* buttonArea);
    void setLogArea(LogArea* logArea);
    void setEnable(bool flag);
    void setCoverArea(HandArea* coverArea){this->coverArea = coverArea;}
    void setBPArea(BPArea* bpArea){this->bpArea = bpArea;}
    void setChatArea(ChatArea*chat){this->chatArea=chat;}
    void setChatLine(ChatLine*chat){this->chatLine=chat;}
    void setTimelineBar(TimelineBar* bar) { this->timelineBar = bar; }
    void reset();
    void alert();
    QWidget* getWindow(){return window;}
    HandArea* getHandArea();
    PlayerArea* getPlayerArea();
    ShowArea* getShowArea();
    TeamArea* getTeamArea();
    TipArea* getTipArea();
    DecisionArea* getDecisionArea();
    ButtonArea* getButtonArea();
    void logAppend(QString msg);
    void chatAppend(int id,QString msg){chatArea->append(id,msg);}
    HandArea* getCoverArea(){return coverArea;}
    BPArea* getBPArea(){return bpArea;}
    ChatLine*getChatLine(){return chatLine;}
    TimelineBar* getTimelineBar() { return this->timelineBar; }
public slots:
    void showCoverArea(bool show);
    void showCoverArea();
    void closeCoverArea();
private:
    QWidget* window;
    HandArea* handArea;
    PlayerArea* playerArea;
    ShowArea* showArea;
    TeamArea* teamArea;
    TipArea* tipArea;
    DecisionArea* decisionArea;
    ButtonArea* buttonArea;
    LogArea* logArea;
    ChatArea* chatArea;
    ChatLine* chatLine;
    HandArea* coverArea;
    BPArea* bpArea;
    TimelineBar *timelineBar;
};
extern GUI* gui;

#endif // GUI_H
