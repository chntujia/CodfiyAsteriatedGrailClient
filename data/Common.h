#ifndef COMMON_H
#define COMMON_H
#include <QString>
enum CAUSE{
    CAUSE_DEFAULT,
    CAUSE_OVERFLOW,
    CAUSE_USE,
    CAUSE_ATTACK,
    CAUSE_POISON,
    CAUSE_WEAKEN,
    CAUSE_MISSILE
};

enum DECK{
    DECK_PILE = 1,
    DECK_DISCARD = 2,
    DECK_HAND = 4,
    DECK_BASIC_EFFECT = 5,
    DECK_COVER = 6
};

enum HARM_TYPE{
    HARM_NONE,
    HARM_ATTACK,
    HARM_MAGIC
};

enum ACTION_FLAG{
    ANY_ACTION,
    ATTACK_ACTION,
    MAGIC_ACTION,
    SPECIAL_ACTION,
    ATTACK_MAGIC,
    NO_ACTION
};

enum HIT_RATE{
    RATE_NORMAL,
    RATE_NOREATTACK,
    RATE_NOMISS
};

enum REATTACK{
    RA_ATTACK,
    RA_BLOCK,
    RA_GIVEUP
};

QString getCauseString(int cause)
{
    switch(cause)
    {
    case CAUSE_DEFAULT:
        return "default";
    case CAUSE_OVERFLOW:
        return "overflow";
    case CAUSE_USE:
        return "use";
    case CAUSE_ATTACK:
        return QStringLiteral("攻击");
    case CAUSE_POISON:
        qDebug("here");
        return QStringLiteral("中毒");
    case CAUSE_WEAKEN:
        return QStringLiteral("虚弱");
    case CAUSE_MISSILE:
        return QStringLiteral("魔弹");
    default:
        return "undefined";
    }
}

#endif // COMMON_H
