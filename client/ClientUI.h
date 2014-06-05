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
    void showMessage(uint16_t proto_type, google::protobuf::Message* proto);
    void displayError(QAbstractSocket::SocketError);

    void UserLogin();
    void UserRegist();
    void UserRegistShow();
    void UserBackLogin();
private:
    Client* tcpSocket;
    Ui::ClientUI *ui;
};

#endif // CLIENTUI_H
