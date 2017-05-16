//#define LOBBY
#include <QMetaType>
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
    ClientUI c;
#ifdef LOBBY
    c.disconnect(&c);
    Lobby l;
    l.show();
    return a.exec();
#else
    if(c.exec() == QDialog::Accepted)
    {
        c.disconnect(&c);
        Lobby l;
        l.show();
        return a.exec();
    }
    return 0;
#endif
}
