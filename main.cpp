#define LOBBY
#include <QtGui>
#include <QApplication>
#include "widget/Animation.h"
#include "widget/RoomView.h"
#include "data/DataInterface.h"
#include "widget/GUI.h"
#include "client/ClientUI.h"
#include "client/Lobby.h"
#include "logic/Logic.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont("simsun", 9));
    dataInterface = new DataInterface;
    logic = new Logic;
    animation = new Animation;
    gui = new GUI;
    //FIXME: use ClientUI login and then destroy it
    ClientUI c;
#ifdef LOBBY
    c.disconnect(&c);
    Lobby l;
    l.show();
    return a.exec();
#else
    c.link();
    if(c.exec() == QDialog::Accepted)
    {
        RoomView view;
        gui->setWindow(view.window());
        animation->setRoomScene(view.getScene());
        view.show();
//GUI初始化完成
        /**/
        logic->readyToStart();
        gui->logAppend(QStringLiteral("请相信这不是卡，这是在等那个还没点Start的二货╮(╯▽╰)╭"));
        return a.exec();
    }
    else
        return 0;
#endif
}
