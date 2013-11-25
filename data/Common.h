#ifndef COMMON_H
#define COMMON_H
#include <QString>

enum GrailError{
    GE_SUCCESS,
    GE_TIMEOUT,
    GE_URGENT,
    GE_EMPTY_HANDLE,
    GE_NO_STATE,
    GE_DECK_OVERFLOW,
    GE_CARD_NOT_ENOUGH,
    GE_HANDCARD_NOT_FOUND,
    GE_BASIC_EFFECT_NOT_FOUND,
    GE_BASIC_EFFECT_ALREADY_EXISTS,
    GE_EXCLUSIVE_EFFECT_NOT_FOUND,
    GE_MOVECARD_FAILED,
    GE_INCONSISTENT_STATE,
    GE_FATAL_ERROR,
    GE_INVALID_PLAYERID,
    GE_INVALID_CARDID,
    GE_INVALID_ACTION,
    GE_INVALID_STEP,
    GE_INVALID_EXCLUSIVE_EFFECT,
    GE_NOT_SUPPORTED,
    GE_INVALID_ARGUMENT,
    GE_NO_CONTEXT,
    GE_NO_REPLY
};

enum CAUSE{
    CAUSE_DEFAULT,
    CAUSE_OVERLOAD,
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
    KUANG_HUA = 203,
    XUE_YING_KUANG_DAO_USED = 204,
    SI_LIE = 205,
	JING_ZHUN_SHE_JI = 301,
	SHAN_GUANG_XIAN_JIN = 302,
	JU_JI = 303,
	SHAN_DIAN_JIAN = 304,
	GUAN_CHUAN_SHE_JI = 305,
    FENG_ZHI_FENG_YIN = 401,
    SHUI_ZHI_FENG_YIN = 402,
    HUO_ZHI_FENG_YIN = 403,
    DI_ZHI_FENG_YIN = 404,
    LEI_ZHI_FENG_YIN = 405,
    FA_SHU_JI_DONG = 406,
    WU_XI_SHU_FU = 407,
    FENG_YIN_PO_SUI = 408,
    FAN_SHI = 501,
    SHUI_YING = 502,
    QIAN_XING = 503,
    ZHI_LIAO_SHU = 601,
    ZHI_YU_ZHI_GUANG = 602,
    BING_SHUANG_DAO_YAN = 603,
    LIAN_MIN = 604,
    SHENG_LIAO = 605,
    TIAN_SHI_ZHI_QIANG = 701,
    MO_BAO_CHONG_JI = 801,
    MO_DAN_ZHANG_WO = 802,
    MO_DAN_RONG_HE = 803,
    HUI_MIE_FENG_BAO = 804,
	HUI_YAO = 1001,
    CHENG_JIE = 1002,
    SHENG_GUANG_QI_YU = 1003,
	TIAN_QIANG = 1004,
	DI_QIANG = 1005,
	SHEN_SHENG_XIN_YANG = 1006,
	SHENG_JI = 1007,
    FENG_REN = 1101,
    BING_DONG = 1102,
    HUO_QIU = 1103,
    YUN_SHI = 1104,
    LEI_JI = 1105,
    YUAN_SU_DIAN_RAN = 1106,
    YUE_GUANG = 1107,
    YUAN_SU_XI_SHOU = 1108,
    QI_ZHA = 1201,
    MAO_XIAN_ZHE_TIAN_TANG = 1202,
    TOU_TIAN_HUAN_RI = 1203,
    TE_SHU_JIA_GONG = 1204,
    BU_XIU = 1301,
    SHENG_DU = 1302,
    WEN_YI = 1303,
    SI_WANG_ZHI_CHU = 1304,
    MU_BEI_YUN_LUO = 1305,
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
    case CAUSE_OVERLOAD:
        return "overload";
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
    case XUE_YING_KUANG_DAO:
        return QStringLiteral("血影狂刀");
    case XUE_XING_PAO_XIAO:
        return QStringLiteral("血腥咆哮");
    case KUANG_HUA:
        return QStringLiteral("狂化");
    case SI_LIE:
        return QStringLiteral("撕裂");
    case XUE_YING_KUANG_DAO_USED:
        return QStringLiteral("血影狂刀");
    case FENG_ZHI_FENG_YIN:
        return QStringLiteral("风之封印");
    case SHUI_ZHI_FENG_YIN:
        return QStringLiteral("水之封印");
    case HUO_ZHI_FENG_YIN:
        return QStringLiteral("火之封印");
    case DI_ZHI_FENG_YIN:
        return QStringLiteral("地址封印");
    case LEI_ZHI_FENG_YIN:
        return QStringLiteral("雷之封印");
    case FA_SHU_JI_DONG:
        return QStringLiteral("法术激荡");
    case WU_XI_SHU_FU:
        return QStringLiteral("五系束缚");
    case FENG_YIN_PO_SUI:
        return QStringLiteral("封印破碎");
    case FAN_SHI:
        return QStringLiteral("反噬");
    case SHUI_YING:
        return QStringLiteral("水影");
    case QIAN_XING:
        return QStringLiteral("潜行");
    case BING_SHUANG_DAO_YAN:
        return QStringLiteral("冰霜祷言");
    case ZHI_LIAO_SHU:
        return QStringLiteral("治疗术");
    case ZHI_YU_ZHI_GUANG:
        return QStringLiteral("治愈之光");
    case LIAN_MIN:
        return QStringLiteral("怜悯");
    case SHENG_LIAO:
        return QStringLiteral("圣疗");
    case MO_BAO_CHONG_JI:
        return QStringLiteral("魔爆冲击");
    case MO_DAN_ZHANG_WO:
        return QStringLiteral("魔弹掌握");
    case MO_DAN_RONG_HE:
        return QStringLiteral("魔弹融合");
    case HUI_MIE_FENG_BAO:
        return QStringLiteral("毁灭风暴");
	case HUI_YAO:
        return QStringLiteral("辉耀");
	case CHENG_JIE:
        return QStringLiteral("惩戒");
	case SHENG_GUANG_QI_YU:
        return QStringLiteral("圣光祈愈");
	case TIAN_QIANG:
        return QStringLiteral("天枪");
	case DI_QIANG:
        return QStringLiteral("地枪");
	case SHEN_SHENG_XIN_YANG:
        return QStringLiteral("神圣信仰");
	case SHENG_JI:
        return QStringLiteral("圣击");
    case FENG_REN:
        return QStringLiteral("风刃");
    case BING_DONG:
        return QStringLiteral("冰冻");
    case HUO_QIU:
        return QStringLiteral("火球");
    case YUN_SHI:
        return QStringLiteral("陨石");
    case LEI_JI:
        return QStringLiteral("雷击");
    case YUAN_SU_DIAN_RAN:
        return QStringLiteral("元素点燃");
    case YUE_GUANG:
        return QStringLiteral("月光");
    case YUAN_SU_XI_SHOU:
        return QStringLiteral("元素吸收");
    case BU_XIU:
        return QStringLiteral("不朽");
    case SHENG_DU:
        return QStringLiteral("圣渎");
    case WEN_YI:
        return QStringLiteral("瘟疫");
    case SI_WANG_ZHI_CHU:
        return QStringLiteral("死亡之触");
    case MU_BEI_YUN_LUO:
        return QStringLiteral("墓碑陨落");
    case QI_ZHA:
        return QStringLiteral("欺诈");
    case MAO_XIAN_ZHE_TIAN_TANG:
        return QStringLiteral("冒险家天堂");
    case TOU_TIAN_HUAN_RI:
        return QStringLiteral("偷天换日");
    case TE_SHU_JIA_GONG:
        return QStringLiteral("特殊加工");
	case JING_ZHUN_SHE_JI:
		return QStringLiteral("精准射击");
	case SHAN_GUANG_XIAN_JIN:
		return QStringLiteral("闪光陷阱");
	case JU_JI:
		return QStringLiteral("狙击");
	case GUAN_CHUAN_SHE_JI:
		return QStringLiteral("贯穿射击");
	
	default:
        return "CauseString undefined";
    }
}

#endif // COMMON_H
