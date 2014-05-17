#ifndef CARD_H
#define CARD_H
#include <QList>
#include <QStringList>
#define EXCLUSIVE_NUM 5
enum Exclusive{
    EX_WU_XI_SHU_FU,
    EX_TIAO_XIN,
    EX_LING_HUN_LIAN_JIE,
    EX_TONG_SHENG_GONG_SI,
    EX_YONG_HENG_YUE_ZHANG
};

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
