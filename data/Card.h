#ifndef CARD_H
#define CARD_H
#include <QList>
#include <QStringList>
class Card
{
public:
    Card(QStringList *cardEntry);
    int getID();
    int getHasSpeciality();
    QString getInfo();
    QString getType();
    QString getElement();
    QString getChineseElement();
    QString getProperty();
    QString getName();
    QString getSource();
    QString getDescription(){return description;}
    QStringList getSpecialityList();
protected:
    int id;
    int hasSpeceiality;
    QString type;
    QString element;
    QString property;
    QString name;
    QString description;
    QStringList specialityList;
    QString source;
};

#endif // CARD_H
/*
enum ELEMENT {feng, shui, huo, di, lei, guang, an};
enum PROPERTY {ji, xue, huan, yong, sheng};
enum TYPE {attack, magic};
*/
