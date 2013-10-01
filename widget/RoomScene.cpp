#include "RoomScene.h"
#include <QPointF>
#include "widget/GUI.h"
#include <QPainter>
static const QPointF HandPos(190,505);
static const QPointF PlayerAreaPos(0,0);
static const QPointF ShowAreaPos(193,260);
static const QPointF TipAreaPos(290,230);
static const QPointF TeamAreaPos(2,3);
static const QPointF DecisionAreaPos(725,450);
static const QPointF ButtonAreaPos(10,650);
static const QPointF LogAreaPos(890,23);
static const QPointF ChatAreaPos(890,398);
static const QPointF ChatLinePos(890,650);
static const QPointF BPAreaPos(0,0);
RoomScene::RoomScene()
{
    setBackgroundBrush(QBrush(QPixmap("resource/background.png")));

    coverArea = new HandArea;
    addItem(coverArea);
    coverArea->setPos(HandPos);
    gui->setCoverArea(coverArea);
    coverArea->disableAll();
    coverArea->setVisible(false);

    handArea = new HandArea;
    addItem(handArea);
    handArea->setPos(HandPos);
    gui->setHandArea(handArea);

    playerArea=new PlayerArea;
    addItem(playerArea);
    playerArea->setPos(PlayerAreaPos);
    gui->setPlayerArea(playerArea);
    playerArea->disableAll();

    showArea=new ShowArea;
    addItem(showArea);
    showArea->setPos(ShowAreaPos);
    gui->setShowArea(showArea);

    tipArea=new TipArea;
    addItem(tipArea);
    tipArea->setPos(TipAreaPos);
    gui->setTipArea(tipArea);

    decisionArea=new DecisionArea;
    addItem(decisionArea);
    decisionArea->setPos(DecisionAreaPos);
    gui->setDecisionArea(decisionArea);

    buttonArea=new ButtonArea;
    addItem(buttonArea);
    buttonArea->setPos(ButtonAreaPos);
    gui->setButtonArea(buttonArea);

    teamArea=new TeamArea();
    addItem(teamArea);
    teamArea->setPos(TeamAreaPos);
    gui->setTeamArea(teamArea);

    logArea=new LogArea;
    logAreaProxy=addWidget(logArea);
    logAreaProxy->setPos(LogAreaPos);
    gui->setLogArea(logArea);

    chatLine=new ChatLine;
    chatLineProxy=addWidget(chatLine);
    chatLineProxy->setPos(ChatLinePos);
    gui->setChatLine(chatLine);    

    chatArea=new ChatArea;
    chatAreaProxy=addWidget(chatArea);
    chatAreaProxy->setPos(ChatAreaPos);
    gui->setChatArea(chatArea);

    bpArea=new BPArea();
    addItem(bpArea);
    bpArea->setPos(BPAreaPos);
    gui->setBPArea(bpArea);

}
