#include "data/DataInterface.h"
#include <QTextStream>
#include <QFile>
#include "widget/GUI.h"

DataInterface *dataInterface=NULL;
DataInterface::DataInterface(QObject *parent) :
    QObject(parent)
{
    initCardDB();
    initRoleList();
    initRoleSkillDB();
    initButtonDB();
}
void DataInterface::initCardDB()
{
    QFile fp("data/cardDB.txt");
    QTextStream in(&fp);
    QTextStream out(stdout);
    if (!fp.open(QIODevice::ReadOnly)) {
            out << "Cannot open file for reading. ";
    }
    cardDB.clear();
    QStringList cardEntry;
    while (!in.atEnd())
    {
        cardEntry=in.readLine().split("\t");
        cardDB<<new Card(&cardEntry);
    }
    fp.close();
}

void DataInterface::initButtonDB()
{
    QFile fp("data/buttonDB.txt");
    QTextStream in(&fp);
    QTextStream out(stdout);
    if (!fp.open(QIODevice::ReadOnly)) {
            out << "Cannot open file for reading. ";
    }
    buttonDB.clear();
    QStringList entry;
    while (!in.atEnd())
    {
        entry=in.readLine().split("\t");
        buttonDB[entry[0]]=entry[1].replace("#","\n");
    }
    fp.close();
}

void DataInterface::initRoleList()
{
    roleList<<NULL;
    roleList<<QStringLiteral("[剑圣]");
    roleList<<QStringLiteral("[狂战士]");
    roleList<<QStringLiteral("[弓之女神]");
    roleList<<QStringLiteral("[封印师]");
    roleList<<QStringLiteral("[暗杀者]");
    roleList<<QStringLiteral("[圣女]");
    roleList<<QStringLiteral("[天使]");
    roleList<<QStringLiteral("[魔导师]");
    roleList<<QStringLiteral("[魔剑]");
    roleList<<QStringLiteral("[圣枪]");
    roleList<<QStringLiteral("[元素师]");
    roleList<<QStringLiteral("[冒险家]");
    roleList<<QStringLiteral("[死灵法师]");
    roleList<<QStringLiteral("[仲裁者]");
    roleList<<QStringLiteral("[神官]");
    roleList<<QStringLiteral("[祈祷师]");
    roleList<<QStringLiteral("[贤者]");
    roleList<<QStringLiteral("[灵符师]");
    roleList<<QStringLiteral("[剑帝]");
    roleList<<QStringLiteral("[格斗家]");
    roleList<<QStringLiteral("[勇者]");
    roleList<<QStringLiteral("[灵魂术士]");
    roleList<<QStringLiteral("[巫女]");
    roleList<<QStringLiteral("[蝶舞者]");
    roleList<<QStringLiteral("[]");
    roleList<<QStringLiteral("[魔弓]");
    roleList<<QStringLiteral("[]");
    roleList<<QStringLiteral("[红莲骑士]");
    roleList<<QStringLiteral("[魔枪]");
}

void DataInterface::initRoleSkillDB()
{
    QFile fp("data/skillDB.txt");
    QTextStream in(&fp);
    QTextStream out(stdout);
    if (!fp.open(QIODevice::ReadOnly)) {
            out << "Cannot open file for reading. ";
    }
    roleSkillDB.clear();
    QStringList entry;
    while (!in.atEnd())
    {
        entry=in.readLine().split("\t");
        roleSkillDB[entry[0].toInt()]=entry[1].replace("#","\n");
    }
    fp.close();
}

void DataInterface::initPlayerList(QString msg,QStringList nicknames)
{
    int i,isRed,pID,myPos;
    queue=msg;
    //find myPos
    for(i=0;i<playerMax;i++)
        if(msg[i].digitValue()==id)
            break;
    myPos=i;
    //设置座次，分队    
    for(;i<playerMax;i++)
    {
        pID=msg[i].digitValue();
        isRed=msg[i+playerMax].digitValue();
        playerList<<new Player(pID,isRed,"");

    }

    pID=msg[0].digitValue();
    firstPlayerID=pID;

    for(i=0;i<myPos;i++)
    {
        pID=msg[i].digitValue();
        isRed=msg[i+playerMax].digitValue();
        playerList<<new Player(pID,isRed,"");

    }
    myself=playerList[0];

}

void DataInterface::initTeam(int moraleMax)
{
    red=new Team(1,moraleMax);
    blue=new Team(0,moraleMax);
    if (myself->getColor()){
        myTeam=red;
        otherTeam=blue;
    }
    else{
        myTeam=blue;
        otherTeam=red;
    }
}

void DataInterface::sortPlayers()
{
    int i,j;
    for(i=0;i<playerMax;i++)
        for(j=0;j<playerMax;j++)
        {
            if(playerList[j]->getID()==i)
            {
                playerList.swap(i,j);
                break;
            }
        }
}
void DataInterface::setID(int id)
{
    this->id=id;
}
void DataInterface::setPlayerMax(int playerMax)
{
    this->playerMax=playerMax;
}

void DataInterface::setMyself(Player* player)
{
    myself=player;
}

void DataInterface::addHandCard(Card* card)
{
    handcards<<card;
    gui->getHandArea()->addCardItem(card);

}

void DataInterface::addCoverCard(Card* card)
{
    coverCards << card;
    gui->getCoverArea()->addCardItem(card);
}

void DataInterface::removeHandCard(Card *card)
{
    handcards.removeOne(card);
    gui->getHandArea()->removeCardItem(card);
}

void DataInterface::removeCoverCard(Card *card)
{
    coverCards.removeOne(card);
    gui->getCoverArea()->removeCardItem(card);
}
Player* DataInterface::getPlayerById(int id)
{
    for(int i = 0;i < playerList.length();i++)
    {
        if(id == playerList[i]->getID())
            return playerList[i];
    }
    return NULL;
}

Card* DataInterface::getCard(int id)
{
    return cardDB.at(id);
}
int DataInterface::getID()
{
    return id;
}
int DataInterface::getPlayerMax()
{
    return playerMax;
}

QList<Player*> DataInterface::getPlayerList()
{
    return playerList;
}

QList<Card*> DataInterface::getHandCards()
{
    return handcards;
}

QList<Card*> DataInterface::getCoverCards()
{
    return coverCards;
}

Player* DataInterface::getMyself()
{
    return myself;
}

Team* DataInterface::getRedTeam()
{
    return red;
}
Team* DataInterface::getBlueTeam()
{
    return blue;
}
