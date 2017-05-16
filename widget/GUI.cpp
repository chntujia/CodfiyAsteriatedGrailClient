#include "widget/GUI.h"
#include "widget/Animation.h"
#include "data/DataInterface.h"
#ifdef SOUND
  #include <QSound>
  #define QSound(x) QSound(x)
#else
  #define QSound(x)
#endif

GUI* gui=NULL;
GUI::GUI(QObject *parent) :
    QObject(parent)
{

}

void GUI::alert(){
    QApplication::alert(window);
    QSound("sound/Warning.wav");
}

QWidget *GUI::newWindow(int playerNum)
{
    RoomView* view = new RoomView;
    gui->setWindow(view->window());
    animation->setRoomScene(view->getScene());
    view->show();
    dataInterface->setPlayerMax(playerNum);
    return view;
}

void GUI::setupRoom(bool isStarted)
{
    playerArea->init(isStarted);
    buttonArea->init(isStarted);
}

void GUI::cleanRoom()
{
    handArea->cleanCardItem();
    coverArea->cleanCardItem();
    chatArea->clear();
}

void GUI::reset()
{
    playerArea->reset();
    buttonArea->reset();
    handArea->reset();
    tipArea->reset();
    decisionArea->reset();

    coverArea->reset();
    showCoverArea(false);
    timelineBar->stopCounting();
}

void GUI::setEnable(bool flag)
{
    reset();
    handArea->setEnabled(flag);
    playerArea->setEnabled(flag);
    buttonArea->setEnabled(flag);
    decisionArea->setEnabled(flag);
}

void GUI::setHandArea(HandArea* handArea)
{
    this->handArea=handArea;
}

void GUI::setPlayerArea(PlayerArea* playerArea)
{
    this->playerArea=playerArea;
}

void GUI::setShowArea(ShowArea* showArea)
{
    this->showArea=showArea;
}

void GUI::setTeamArea(TeamArea* teamArea)
{
    this->teamArea=teamArea;
}

void GUI::setTipArea(TipArea* tipArea)
{
    this->tipArea=tipArea;
}
void GUI::setDecisionArea(DecisionArea* decisionArea)
{
    this->decisionArea=decisionArea;
}

void GUI::setButtonArea(ButtonArea* buttonArea)
{
    this->buttonArea=buttonArea;
}

void GUI::setLogArea(LogArea *logArea)
{
    this->logArea=logArea;
}

HandArea* GUI::getHandArea()
{
    return handArea;
}

PlayerArea* GUI::getPlayerArea()
{
    return playerArea;
}

ShowArea* GUI::getShowArea()
{
    return showArea;
}

TeamArea* GUI::getTeamArea()
{
    return teamArea;
}

TipArea* GUI::getTipArea()
{
    return tipArea;
}

DecisionArea* GUI::getDecisionArea()
{
    return decisionArea;
}

ButtonArea* GUI::getButtonArea()
{
    return buttonArea;
}

void GUI::logAppend(QString msg)
{
    logArea->append(msg);
}

void GUI::showCoverArea(bool show)
{
    this->handArea->setVisible(!show);
    this->coverArea->setVisible(show);
}

void GUI::showCoverArea()
{
    this->handArea->setVisible(false);
    this->coverArea->setVisible(true);
}

void GUI::closeCoverArea()
{
    this->coverArea->setVisible(false);
    this->handArea->setVisible(true);
}

void GUI::hideBP()
{
    bpArea->hide();
}
