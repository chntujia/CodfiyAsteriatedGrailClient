#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <client/Client.h>
#include <client/Lobby.h>
class Role;
class Logic : public QObject
{
    Q_OBJECT
public:
    explicit Logic(QObject *parent = 0);
    void setupRoom(bool isStarted, network::GameInfo* gameInfo);
    void cleanRoom();
    void destroyRoom();
    void makeConnection();
    void setClient(Client* socket){this->socket=socket;}
    void setLobby(Lobby* l) { lobby = l; }
    void setMyRole(int roleID=0);
    Client* getClient(){return socket;}
signals:
    void sendCommand(uint16_t proto_type, google::protobuf::Message* proto);  
public slots:
    void onOkClicked();
    void getCommand(uint16_t proto_type, google::protobuf::Message* proto);
    void roleAnalyse();
    void onButtonClicked(int id);
    void onButtonUnclicked(int id);
private:
    Client* socket;
    Lobby* lobby;
    int count;
    int state;    
    int myID;
    bool hasShownRole;
    bool hasSetRole;
    Role* myRole;
    bool init_before_start;
    bool init_after_start;
    int roles[8];
};
extern Logic* logic;
#endif // LOGIC_H
