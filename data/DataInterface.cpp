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

    red = new Team(1);
    blue = new Team(0);
    gameInfo = NULL;
}

void DataInterface::cleanRoom()
{
    red->setCrystal(0);
    red->setGem(0);
    red->setGrail(0);
    red->setMorale(0);
    blue->setCrystal(0);
    blue->setGem(0);
    blue->setGrail(0);
    blue->setMorale(0);
    handcards.clear();
    coverCards.clear();
}

void DataInterface::setupRoom(bool isStarted)
{
    if(!isStarted){
        initDefaultPlayerList();
    }
    else{
        if (playerMax==8)
            initTeam(18);
        else
            initTeam(15);
    }
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
    roleList[0] = QStringLiteral("角色");
    roleList[1] = QStringLiteral("[剑圣]");
    roleList[2] = QStringLiteral("[狂战士]");
    roleList[3] = QStringLiteral("[弓之女神]");
    roleList[4] = QStringLiteral("[封印师]");
    roleList[5] = QStringLiteral("[暗杀者]");
    roleList[6] = QStringLiteral("[圣女]");
    roleList[7] = QStringLiteral("[天使]");
    roleList[8] = QStringLiteral("[魔导师]");
    roleList[9] = QStringLiteral("[魔剑]");
    roleList[10] = QStringLiteral("[圣枪]");
    roleList[11] = QStringLiteral("[元素师]");
    roleList[12] = QStringLiteral("[冒险家]");
    roleList[13] = QStringLiteral("[死灵法师]");
    roleList[14] = QStringLiteral("[仲裁者]");
    roleList[15] = QStringLiteral("[神官]");
    roleList[16] = QStringLiteral("[祈祷师]");
    roleList[17] = QStringLiteral("[贤者]");
    roleList[18] = QStringLiteral("[灵符师]");
    roleList[19] = QStringLiteral("[剑帝]");
    roleList[20] = QStringLiteral("[格斗家]");
    roleList[21] = QStringLiteral("[勇者]");
    roleList[22] = QStringLiteral("[灵魂术士]");
    roleList[23] = QStringLiteral("[巫女]");
    roleList[24] = QStringLiteral("[蝶舞者]");
    roleList[25] = QStringLiteral("[女武神]");
    roleList[26] = QStringLiteral("[魔弓]");
    roleList[27] = QStringLiteral("[英灵人形]");
    roleList[28] = QStringLiteral("[红莲骑士]");
    roleList[29] = QStringLiteral("[魔枪]");
    roleList[30] = QStringLiteral("[苍炎魔女]");
    roleList[31] = QStringLiteral("[吟游诗人]");
    roleList[108] = QStringLiteral("[sp魔导师]");
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

void DataInterface::initDefaultPlayerList()
{
    int i, myPos;
    myPos = id == GUEST ? 0 : id;
    foreach(Player* p, playerList)
        delete p;
    playerList.clear();
    //设置座次，分队
    for(i = myPos; i < playerMax; i++)
    {
        playerList << new Player(i, -1, "");
    }

    firstPlayerID = 0;

    for(i = 0; i < myPos; i++)
    {
        playerList << new Player(i, -1, "");

    }
    myself = playerList[0];
}

void DataInterface::initPlayerList(network::GameInfo* game_info)
{
    int i,isRed,pID,myPos;
    if(gameInfo != NULL)
        delete gameInfo;
    gameInfo = new network::GameInfo;
    gameInfo->CopyFrom(*game_info);
    network::SinglePlayerInfo player;
    int myViewPoint = id == GUEST ? 0 : id;
    //find myPos
    for(i=0;i<playerMax;i++)
        if(game_info->player_infos(i).id() == myViewPoint)
            break;
    myPos=i;
    foreach(Player* ptr, playerList)
        delete ptr;
    playerList.clear();
    //设置座次，分队    
    for(;i<playerMax;i++)
    {
        player = game_info->player_infos(i);
        pID = player.id();
        isRed = player.team();
        playerList<<new Player(pID, isRed, nickNameList[pID]);
    }

    pID=game_info->player_infos(0).id();
    firstPlayerID=pID;

    for(i=0;i<myPos;i++)
    {
        pID=game_info->player_infos(i).id();
        isRed=game_info->player_infos(i).team();
        playerList<<new Player(pID, isRed, nickNameList[pID]);

    }
    myself=playerList[0];

}

void DataInterface::initTeam(int moraleMax)
{
    red->setMoraleMax(moraleMax);
    blue->setMoraleMax(moraleMax);

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

void DataInterface::cleanHandCard()
{
    handcards.clear();
    gui->getHandArea()->cleanCardItem();
}

void DataInterface::cleanCoverCard()
{
    coverCards.clear();
    gui->getCoverArea()->cleanCardItem();
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

SafeList<Player*> DataInterface::getPlayerList()
{
    return playerList;
}

SafeList<Card*> DataInterface::getHandCards()
{
    return handcards;
}

SafeList<Card*> DataInterface::getCoverCards()
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
