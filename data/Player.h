#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsObject>
#include "data/Card.h"
class BasicStatus
{
    friend class StatusItem;
    friend class Player;
public:
    BasicStatus(int type,Card*card){this->type=type;this->card=card;}
    Card* getCard(){return card;}
private:
    int type;
    Card*card;
};

class Token
{
    friend class StatusItem;
    friend class Player;
public:
    Token(QString name,int max,int type):tokenName(name),max(max),num(0),type(type){}
private:
    QString tokenName;
    int max;
    int num;
    int type;
};

class Player:public QObject
{
    Q_OBJECT
public:
    Player(int id,int color,QString nickname);
    void setHandCardsMax(int howMany);
    void setCoverCardsMax(int howMany){this->coverCardsMax = howMany;}
    void setCrossNum(int howMany);
    void setGem(int howMany);
    void setCrystal(int howMany);
    void setEnergyMax(int howMany);
    void setRole(int roleID);
    void setTap(bool flag){tap=flag;}
    void setSpecial(int type,bool flag);
    void setToken(int id,int howMany){token[id]->num=howMany;}
    void addToken(int id,Token *token){this->token[id]=token;emit addTokenSIG(token);}
    void changeHandCardNum(int increase){handCardsNum+=increase;}
    void changeCoverCardNum(int increase){coverCardsNum += increase;}
    void addBasicStatus(int type,Card* card);
    void removeBasicStatus(Card* card);
    bool checkBasicStatus(int type);
    bool checkBasicStatus(QString status);
    bool hasStatus(){return !BasicStatusList.isEmpty();}
    void setNickname(QString name){this->nickname=name;}
    int getID();
    QString getNickname(){return nickname;}
    QString getRoleName(){return roleName;}
    int getHandCardMax();
    int getHandCardNum();
    int getCoverCardMax(){return this->coverCardsMax;}
    int getCoverCardNum(){return this->coverCardsNum;}
    int getCrossNum();
    int getCrossMax();
    int getGem();
    int getCrystal();
    int getEnergy();
    int getEnergyMax();
    int getColor();
    int getRoleID(){return roleID;}
    int getToken(int id){return token[id]->num;}
    bool getSpecial(int type){return specials[type];}
    bool getTap(){return tap;}
    QString getFaceSource(){return faceSource;}
    QString getTapSource(){return tapSource;}
    QList<BasicStatus*> getBasicStatus(){return BasicStatusList;}
    void setPos(int pos){this->pos = pos;}
    int getPos(){return pos;}
signals:
    void addBasicStatusSIG(BasicStatus *status);
    void removeBasicStatusSIG(BasicStatus *status);
    void addSpecialStatusSIG(int type);
    void removeSpecialStatusSIG(int type);
    void addTokenSIG(Token*);
protected:
    int id;
    int pos;
    QString nickname;
    QString roleName;
    int handCardsMax;
    int handCardsNum;
    int coverCardsMax;
    int coverCardsNum;
    int crossNum;
    int crossMax;
    int gem;
    int crystal;
    int energyMax;
    int color;
    int roleID;    
    bool tap;
    bool specials[5];
    QList<BasicStatus*> BasicStatusList;
    Token* token[3];
    QString faceSource;
    QString tapSource;
};


#endif // PLAYER_H
