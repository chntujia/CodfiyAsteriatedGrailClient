#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include "codec.h"
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
    void getMessage(unsigned short proto_type, google::protobuf::Message* proto);
public slots:
    void readMessage();
    void sendMessage(unsigned short proto_type, google::protobuf::Message* proto);
    void onDisconnected();
private:
    QString m_addr;
    QString nickname;
    int isRed;
    quint32 nextBlockSize;
};

#endif // CLIENT_H
