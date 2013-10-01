#ifndef ROOMSCENE_H
#define ROOMSCENE_H

#include <QGraphicsScene>
#include "HandArea.h"
#include "PlayerArea.h"
#include "TeamArea.h"
#include "ShowArea.h"
#include "TipArea.h"
#include "ButtonArea.h"
#include "LogArea.h"
#include "BPArea.h"
#include <QGraphicsProxyWidget>
class RoomScene : public QGraphicsScene
{
    Q_OBJECT

public:
    RoomScene();

private:
    HandArea * handArea;
    PlayerArea* playerArea;
    ShowArea* showArea;
    TeamArea *teamArea;
    TipArea* tipArea;
    DecisionArea* decisionArea;
    ButtonArea* buttonArea;
    LogArea* logArea;
    QGraphicsProxyWidget* logAreaProxy;
    ChatArea* chatArea;
    QGraphicsProxyWidget* chatAreaProxy;
    ChatLine* chatLine;
    QGraphicsProxyWidget* chatLineProxy;

    HandArea *coverArea;
    BPArea* bpArea;
};
#endif // ROOMSCENE_H
