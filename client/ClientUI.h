#ifndef CLIENTUI_H
#define CLIENTUI_H

#include <QDialog>
#include <QtNetwork>
#include "client/Client.h"

namespace Ui {
class ClientUI;
}
class Client;
class ClientUI : public QDialog
{
    Q_OBJECT

public:
    explicit ClientUI(QWidget *parent = 0);
    ~ClientUI();


public slots:
    void showMessage(unsigned short proto_type, google::protobuf::Message* proto);
    void displayError(QAbstractSocket::SocketError);
    void GuestLogin();
    void UserLogin();
    void UserRegist();
    void UserRegistShow();
    void UserBackLogin();
	void CheckSavePwd();
	void CheckSaveUser();
private:
    Client* tcpSocket;
    Ui::ClientUI *ui;
};

#endif // CLIENTUI_H
