#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include <QObject>
#include "data/Player.h"
#include "data/Team.h"
#include "data/Card.h"
#include "protocol/action_respond.pb.h"
#include "protocol/base.pb.h"

#define GUEST 9

class DataInterface : public QObject
{
    Q_OBJECT
public:
    explicit DataInterface(QObject *parent = 0);    
    void initCardDB();
    void initDefaultPlayerList();
    void initPlayerList(network::GameInfo* game_info);
    void initRoleList();
    void initRoleSkillDB();
    void initButtonDB();
    void initTeam(int moraleMax);
    void sortPlayers();
    void addCoverCard(Card* card);
    void addHandCard(Card* card);
    void cleanHandCard();
    void cleanCoverCard();
    void removeHandCard(Card *card);
    void removeCoverCard(Card *card);
    void setMyself(Player* player);
    void setID(int id);
    void setPlayerMax(int playerMax);
    void setNickName(int id, QString name) { if(id>=0 && id<playerMax) nickNameList[id] = name; playerList[id]->setNickname(name);}
    void cleanRoom();
    void setupRoom(bool isStarted);
    Player* getPlayerById(int id);
    Card* getCard(int id);
    int getID();
    int getFirstPlayerID(){return firstPlayerID;}
    Player* getMyself();
    Team* getMyTeam(){return myTeam;}
    Team* getOtherTeam(){return otherTeam;}
    int getPlayerMax();
    QList<Card*> getHandCards();
    QList<Card*> getCoverCards();
    QList<Player*> getPlayerList();
    Team* getRedTeam();
    Team* getBlueTeam();
    network::GameInfo* getGameInfo(){return gameInfo;}
    QString getRoleName(int roleID){return roleList[roleID];}
    QString getButtonInfo(QString skill){return buttonDB[skill];}
    QString getRoleSkillInfo(int roleID){return roleSkillDB[roleID];}
private:
    QList<Card*> cardDB;
    QMap<QString,QString> buttonDB;
    QMap<int,QString> roleSkillDB;
    QList<Player*> playerList;
    QList<Card*> handcards;
    QList<Card*> coverCards;
    Player* myself;
    Team *red,*blue,*otherTeam,*myTeam;
    QStringList roleList;
    QString nickNameList[8];
    int id;
    int playerMax;
    int firstPlayerID;
    network::GameInfo* gameInfo;
};
extern DataInterface *dataInterface;
#endif // DATAINTERFACE_H
