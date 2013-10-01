#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include <QObject>
#include "data/Player.h"
#include "data/Team.h"
#include "data/Card.h"

class DataInterface : public QObject
{
    Q_OBJECT
public:
    explicit DataInterface(QObject *parent = 0);    
    void initCardDB();
    void initPlayerList(QString msg, QStringList nicknames);
    void initRoleList();
    void initRoleSkillDB();
    void initButtonDB();
    void initTeam(int moraleMax);
    void sortPlayers();
    void addCoverCard(Card* card);
    void addHandCard(Card* card);

    void removeHandCard(Card *card);
    void removeCoverCard(Card *card);
    void setMyself(Player* player);
    void setID(int id);
    void setPlayerMax(int playerMax);
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
    QString getQueue(){return queue;}
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
    QString queue;
    QStringList roleList;
    int id;
    int playerMax;
    int firstPlayerID;

};
extern DataInterface *dataInterface;
#endif // DATAINTERFACE_H
