#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <client/Client.h>
#include <client/Lobby.h>

enum ACCOUNT_STATUS;
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
    void setClient(Client* socket){
        this->socket = socket;
        makeConnection();
    }

    void setLobby(Lobby* l) { lobby = l; }
    void setMyRole(int roleID=0);
    void onError(int error);
    void normal();
    void ready();
    void joinTeam();
    Client* getClient(){return socket;}
    ACCOUNT_STATUS getIdentity() const;
    void setIdentity(const ACCOUNT_STATUS &value);
    void muteUser(int userId);
    void unmuteUser(int userId);
    bool isMuted(int userId);
signals:
    void sendCommand(unsigned short proto_type, google::protobuf::Message* proto);
public slots:
    void onOkClicked();
    void onCancelClicked();
    void getCommand(unsigned short proto_type, google::protobuf::Message* proto);
    void roleAnalyse();
    void onButtonClicked(int id);
    void onButtonUnclicked(int id);
private:
    Client* socket;
    Lobby* lobby;
    ACCOUNT_STATUS identity;
    int state;    
    int myID;
    bool hasShownRole;
    Role* myRole;
    bool init_before_start;
    bool init_after_start;
    int roles[8];
    QSet<int> muteList;
};
extern Logic* logic;
#endif // LOGIC_H
