#ifndef COMMON_H
#define COMMON_H
#include <QString>
enum GrailError{
    GE_SUCCESS,
    GE_TIMEOUT,
    GE_URGENT,
    GE_URGENT_AND_DONE,
    GE_EMPTY_HANDLE,
    GE_NO_STATE,
    GE_DECK_OVERFLOW,
    GE_CARD_NOT_ENOUGH,
    GE_HANDCARD_NOT_FOUND,
    GE_BASIC_EFFECT_NOT_FOUND,
    GE_BASIC_EFFECT_ALREADY_EXISTS,
    GE_MOVECARD_FAILED,
    GE_INCONSISTENT_STATE,
    GE_FATAL_ERROR,
    GE_INVALID_PLAYERID,
    GE_INVALID_CARDID,
    GE_INVALID_ACTION,
    GE_INVALID_STEP,
    GE_NOT_SUPPORTED,
    GE_INVALID_ARGUMENT,
    GE_NO_CONTEXT,
    GE_NO_REPLY
};

enum CAUSE{
    CAUSE_DEFAULT,
    CAUSE_OVERFLOW,
    CAUSE_USE,
    CAUSE_ATTACK,
    CAUSE_POISON,
    CAUSE_WEAKEN,
    CAUSE_MISSILE,
    CAUSE_BUY,
    CAUSE_SYNTHESIZE,
    JI_FENG_JI = 101,
    LIE_FENG_JI = 102,
    LIAN_XU_JI = 103,
    SHENG_JIAN = 104,
    JIAN_YING = 105,
    XUE_YING_KUANG_DAO = 201,
    XUE_XING_PAO_XIAO = 202,
    JING_ZHUN_SHE_JI = 301,
    SHAN_GUANG_XIAN_JIN = 302,
    FENG_ZHI_FENG_YIN = 401,
    SHUI_ZHI_FENG_YIN = 402,
    HUO_ZHI_FENG_YIN = 403,
    DI_ZHI_FENG_YIN = 404,
    LEI_ZHI_FENG_YIN = 405,
    ZHI_LIAO_SHU = 601,
    ZHI_YU_ZHI_GUANG = 602,
    TIAN_SHI_ZHI_QIANG = 701,
    FENG_REN = 1101,
    BING_DONG = 1102,
    HUO_QIU = 1103,
    YUN_SHI = 1104,
    LEI_JI = 1105,
    XUN_JIE_CI_FU = 1601,
    WEI_LI_CI_FU = 1602,
    LING_HUN_ZHEN_BAO = 2201,
    LING_HUN_CI_YU = 2202,
    XUE_ZHI_BEI_MING = 2301
};

enum CHANGE{
    CHANGE_ADD,
    CHANGE_REMOVE
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

enum SpecialActionId{
    SPECIAL_BUY,
    SPECIAL_SYNTHESIZE,
    SPECIAL_EXTRACT
};

enum STEP{
    STEP_INIT = 0,
    STEP_DONE = 9999
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
        return QStringLiteral("中毒");
    case CAUSE_WEAKEN:
        return QStringLiteral("虚弱");
    case CAUSE_MISSILE:
        return QStringLiteral("魔弹");
    case JI_FENG_JI:
        return QStringLiteral("疾风技");
    case LIE_FENG_JI:
        return QStringLiteral("烈风技");
    case LIAN_XU_JI:
        return QStringLiteral("连续技");
    case SHENG_JIAN:
        return QStringLiteral("圣剑");
    case JIAN_YING:
        return QStringLiteral("剑影");
    default:
        return "undefined";
    }
}

#endif // COMMON_H
