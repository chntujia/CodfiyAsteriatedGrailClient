#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include "client/ClientUI.h"

class Client : public QTcpSocket
{
    Q_OBJECT
    friend class ClientUI;
    friend class Logic;
public:
    explicit Client();
    void link(QString addr,int port);
    QString GetAddr() { return m_addr; }
signals:
    void readyToStart();
    void getMessage(QString message);
public slots:
    void readMessage();
    void sendMessage(QString message);
    void onDisconnected();
private:
    QString m_addr;
    QString nickname;
    int isRed;
    bool forwarded;
};

#endif // CLIENT_H
