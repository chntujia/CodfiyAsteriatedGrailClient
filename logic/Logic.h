#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <client/Client.h>

class Logic : public QObject
{
    Q_OBJECT
public:
    explicit Logic(QObject *parent = 0);
    void setClient(Client* socket){this->socket=socket;}
    void setMyRole(int roleID=0);
    void readyToStart();
    Client* getClient(){return socket;}
signals:
    void sendCommand(quint16 proto_type, google::protobuf::Message* proto);
    void gameStart();    
public slots:
    void onOkClicked();
    void getCommand(quint16 proto_type, google::protobuf::Message* proto);
    void delete_proto(quint16 type, google::protobuf::Message *proto);
    void roleAnalyse();
private:
    Client* socket;
    int count;
    int state;
    int roles[8];
    int myID;
    bool hasShownRole;
};
extern Logic* logic;
#endif // LOGIC_H
