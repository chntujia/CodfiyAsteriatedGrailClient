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
    QString nickName;
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
    void setNickName(int id, QString name) {
        if(id>=0 && id<playerMax){
            nickNameList[id] = name;
            playerList[id]->setNickname(name);
        }
    }
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
    SafeList<Card*> getHandCards();
    SafeList<Card*> getCoverCards();
    SafeList<Player*> getPlayerList();
    Team* getRedTeam();
    Team* getBlueTeam();
    network::GameInfo* getGameInfo(){return gameInfo;}
    QString getRoleName(int roleID){return roleList[roleID];}
    QString getButtonInfo(QString skill){return buttonDB.contains(skill) ? buttonDB[skill] : skill;}
    QString getRoleSkillInfo(int roleID){return roleSkillDB[roleID];}
private:
    SafeList<Card*> cardDB;
    QMap<QString,QString> buttonDB;
    QMap<int,QString> roleSkillDB;
    SafeList<Player*> playerList;
    SafeList<Card*> handcards;
    SafeList<Card*> coverCards;
    Player* myself;
    Team *red,*blue,*otherTeam,*myTeam;
    QMap<int,QString> roleList;
    QString nickNameList[8];

    int id;
    int playerMax;
    int firstPlayerID;
    network::GameInfo* gameInfo;
};
extern DataInterface *dataInterface;
#endif // DATAINTERFACE_H
