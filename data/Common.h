﻿#ifndef COMMON_H
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
    GE_COVERCARD_NOT_FOUND,
    GE_MOVECARD_FAILED,
    GE_INCONSISTENT_STATE,
    GE_FATAL_ERROR,
    GE_INVALID_PLAYERID,
    GE_INVALID_CARDID,
    GE_INVALID_ROLEID,
    GE_INVALID_ACTION,
    GE_INVALID_STEP,
    GE_INVALID_EXCLUSIVE_EFFECT,
    GE_NOT_SUPPORTED,
    GE_INVALID_ARGUMENT,
    GE_NO_CONTEXT,
    GE_NO_REPLY,
    GE_DISCONNECTED
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
	SHAN_GUANG_XIAN_JING = 302,
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
    TIAN_SHI_ZHU_FU = 702,
    FENG_ZHI_JIE_JING = 703,
    TIAN_SHI_JI_BAN = 704,
    TIAN_SHI_ZHI_GE = 705,
    SHEN_ZHI_BI_HU = 706,
    MO_BAO_CHONG_JI = 801,
    MO_DAN_ZHANG_WO = 802,
    MO_DAN_RONG_HE = 803,
    HUI_MIE_FENG_BAO = 804,
    XIU_LUO_LIAN_ZHAN=901,
    AN_YING_NING_JU=902,
    AN_YING_ZHI_LI=903,
    AN_YING_KANG_JU=904,
    AN_YING_LIU_XING=905,
    HEI_AN_ZHEN_CHAN=906,
    HEI_AN_ZHEN_CHAN_BU_PAI=9061,
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
    YI_SHI_ZHONG_DUAN = 1401,
    ZHONG_CAI_YI_SHI = 1402,
    MO_RI_SHEN_PAN = 1403,
    SHEN_PAN_LANG_CHAO = 1404,
    PAN_JUE_TIAN_PING = 1405,
    SHEN_SHENG_QI_SHI = 1501,
    SHEN_SHENG_QI_FU = 1502,
    SHUI_ZHI_SHEN_LI = 1503,
    SHENG_SHI_SHOU_HU = 1504,
    SHEN_SHENG_QI_YUE = 1505,
    SHEN_SHENG_LING_YU = 1506,
    SHUI_ZHI_SHEN_LI_GIVE = 1531,
    SHUI_ZHI_SHEN_LI_CROSS = 1532,
    XUN_JIE_CI_FU = 1601,
    WEI_LI_CI_FU = 1602,
    QI_DAO = 1603,
    GUANG_HUI_XIN_YANG = 1604,
    QI_HEI_XIN_YANG = 1605,
    FA_LI_CHAO_XI = 1606,
    ZHI_HUI_FA_DIAN = 1701,
    MO_DAO_FA_DIAN = 1702,
    SHENG_JIE_FA_DIAN = 1703,
    FA_SHU_FAN_TAN = 1704,
    JIAN_HUN_SHOU_HU=1901,
    YANG_GON=1902,
    JIAN_QI_ZHAN=1903,
    TIAN_SHI_YU_E_MO=1904,
    TIAN_SHI_ZHI_HUN=1905,
    E_MO_ZHI_HUN=1906,
    BU_QU_YI_ZHI=1907,
    NIAN_QI_LI_CHANG = 2001,
    XU_LI_YI_JI = 2002,
    NIAN_DAN = 2003,
    NIAN_DAN_SELF = 2031,
    BAI_SHI_HUAN_LONG_QUAN = 2004,
    CANG_YAN_ZHI_HUN = 2005,
    DOU_SHEN_TIAN_QU = 2006,
    XU_LI_CANG_YAN = 2025,
    BAI_SHI_DOU_SHEN = 2046,
    LING_HUN_ZHEN_BAO =2201,
    LING_HUN_CI_YU =2202,
    LING_HUN_ZENG_FU=2203,
    LING_HUN_TUN_SHI=2204,
    LING_HUN_ZHAO_HUAN=2205,
    LING_HUN_ZHUAN_HUAN=2206,
    LING_HUN_JING_XIANG=2207,
    LING_HUN_LIAN_JIAN=2208,
    LING_HUN_LIAN_JIE_REACT=2209,
    XUE_ZHI_BEI_MING = 2301,
    TONG_SHENG_GONG_SI = 2302,
    XUE_ZHI_AI_SHANG = 2303,
    NI_LIU = 2304,
    XUE_ZHI_ZU_ZHOU = 2351,
    XUE_ZHI_ZU_ZHOU_QI_PAI = 2352,
    LIU_XUE=2306,    //added by WTF
    SHENG_MING_ZHI_HUO=2401,
    WU_DONG=2402,
    DU_FEN=2403,
    CHAO_SHENG=2404,
    JING_HUA_SHUI_YUE=2405,
    DIAO_LING=2406,
    YONG_HUA=2407,
    DAO_NI_ZHI_DIE=2408,
    MO_GUAN_CHONG_JI=2601,
    MO_GUAN_CHONG_JI_HIT=26011,
    LEI_GUANG_SAN_SHE =2602,
    LEI_GUANG_SAN_SHE_EXTRA=26021,
    DUO_CHONG_SHE_JI=2603,
    CHONG_NENG=2604,
    CHONG_NENG_GAI_PAI=26041,
    MO_YAN=2605,
    MO_YAN_GAI_PAI=26051,
    CHONG_NENG_MO_YAN=2606,
	XING_HONG_SHENG_YUE = 2801,
    XING_HONG_XIN_YANG = 2802,
	XUE_XING_DAO_YAN = 2803,
    XUE_XING_DAO_YAN_1 = 2804,
	XUE_XING_DAO_YAN_2 = 2805,
	SHA_LU_SHENG_YAN = 2806,
	RE_XUE_FEI_TENG = 2807,
	JIE_JIAO_JIE_ZAO = 2808,
	JIE_JIAO_JIE_ZAO_AFTER_MAGIC = 28081,
	XING_HONG_SHI_ZI =2809,
    AN_ZHI_JIE_FANG=2901,
    HUAN_YING_XING_CHEN=2902,
    HEI_AN_SHU_FU=2903,
    AN_ZHI_ZHANG_BI=2904,
    CHONG_YING=2905,
    CHONG_YING_DISCARD=29051,
    QI_HEI_ZHI_QIANG=2906
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
    case TIAN_SHI_ZHI_QIANG:
        return QStringLiteral("天使之墙");
    case TIAN_SHI_ZHU_FU:
        return QStringLiteral("天使祝福");
    case FENG_ZHI_JIE_JING:
        return QStringLiteral("风之洁净");
    case TIAN_SHI_JI_BAN:
        return QStringLiteral("天使羁绊");
    case TIAN_SHI_ZHI_GE:
        return QStringLiteral("天使之歌");
    case SHEN_ZHI_BI_HU:
        return QStringLiteral("神之庇护");
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
    case YI_SHI_ZHONG_DUAN:
        return QStringLiteral("仪式中断");
    case ZHONG_CAI_YI_SHI:
        return QStringLiteral("仲裁仪式");
    case MO_RI_SHEN_PAN:
        return QStringLiteral("末日审判");
    case SHEN_PAN_LANG_CHAO:
        return QStringLiteral("审判浪潮");
    case PAN_JUE_TIAN_PING:
        return QStringLiteral("判决天平");
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
	case SHAN_GUANG_XIAN_JING:
		return QStringLiteral("闪光陷阱");
	case JU_JI:
		return QStringLiteral("狙击");
	case GUAN_CHUAN_SHE_JI:
		return QStringLiteral("贯穿射击");
	case SHAN_DIAN_JIAN:
		return QStringLiteral("闪电箭");	
    case SHEN_SHENG_QI_SHI:
        return QStringLiteral("神圣启示");
    case SHEN_SHENG_QI_FU:
        return QStringLiteral("神圣祈福");
    case SHUI_ZHI_SHEN_LI:
        return QStringLiteral("水之神力");
    case SHUI_ZHI_SHEN_LI_GIVE:
        return QStringLiteral("水之神力给牌");
    case SHENG_SHI_SHOU_HU:
        return QStringLiteral("圣使守护");
    case SHEN_SHENG_QI_YUE:
        return QStringLiteral("神圣契约");
    case SHEN_SHENG_LING_YU:
        return QStringLiteral("神圣领域");
    case  XIU_LUO_LIAN_ZHAN:
        return QStringLiteral("修罗连斩");
    case  AN_YING_NING_JU:
        return QStringLiteral("暗影凝聚");
    case    AN_YING_ZHI_LI:
        return QStringLiteral("暗影之力");
    case     AN_YING_KANG_JU:
        return QStringLiteral("暗影抗拒");
    case     AN_YING_LIU_XING:
        return QStringLiteral("暗影流星");
    case     HEI_AN_ZHEN_CHAN:
        return QStringLiteral("黑暗震颤");
    case ZHI_HUI_FA_DIAN:
        return QStringLiteral("智慧法典");
    case MO_DAO_FA_DIAN:
        return QStringLiteral("魔道法典");
    case SHENG_JIE_FA_DIAN:
        return QStringLiteral("圣洁法典");
    case FA_SHU_FAN_TAN:
        return QStringLiteral("法术反弹");
    case NIAN_QI_LI_CHANG:
        return QStringLiteral("念气立场");
    case XU_LI_YI_JI:
        return QStringLiteral("蓄力一击");
    case NIAN_DAN:
        return QStringLiteral("念弹");
    case BAI_SHI_HUAN_LONG_QUAN:
        return QStringLiteral("百式幻龙拳");
    case CANG_YAN_ZHI_HUN:
        return QStringLiteral("苍炎之魂");
    case DOU_SHEN_TIAN_QU:
        return QStringLiteral("斗神天驱");
    case XU_LI_CANG_YAN:
        return QStringLiteral("蓄力一击/苍炎之魂");
    case BAI_SHI_DOU_SHEN:
        return QStringLiteral("百式幻龙拳/斗神天驱");
    case XUE_ZHI_BEI_MING:
        return QStringLiteral("血之悲鸣");
    case TONG_SHENG_GONG_SI:
        return QStringLiteral("同生共死");
    case XUE_ZHI_AI_SHANG:
        return QStringLiteral("血之哀伤");
    case NI_LIU:
        return QStringLiteral("逆流");
    case XUE_ZHI_ZU_ZHOU:
        return QStringLiteral("血之诅咒");
    case XUE_ZHI_ZU_ZHOU_QI_PAI:
        return QStringLiteral("血之诅咒弃牌");
    case LIU_XUE:
        return QStringLiteral("流血");
	case QI_DAO:
		return QStringLiteral("祈祷");
    case WEI_LI_CI_FU:
        return QStringLiteral("威力赐福");
    case XUN_JIE_CI_FU:
        return QStringLiteral("迅捷赐福");
    case GUANG_HUI_XIN_YANG:
        return QStringLiteral("光辉信仰");
    case QI_HEI_XIN_YANG:
        return QStringLiteral("漆黑信仰");
    case FA_LI_CHAO_XI:
        return QStringLiteral("法力潮汐");
    case DUO_CHONG_SHE_JI:
        return QStringLiteral("多重射击");
    case JIAN_QI_ZHAN:
        return QStringLiteral("剑气斩");
    case BU_QU_YI_ZHI:
        return QStringLiteral("不屈意志");
	case XING_HONG_SHENG_YUE:
        return QStringLiteral("猩红圣约");
	case XING_HONG_XIN_YANG:
        return QStringLiteral("猩红信仰");
	case XUE_XING_DAO_YAN:
        return QStringLiteral("血腥祷言");
	case SHA_LU_SHENG_YAN:
        return QStringLiteral("杀戮盛宴");
	case RE_XUE_FEI_TENG:
        return QStringLiteral("热血沸腾");
	case JIE_JIAO_JIE_ZAO:
        return QStringLiteral("戒骄戒躁");
	case JIE_JIAO_JIE_ZAO_AFTER_MAGIC:
        return QStringLiteral("戒骄戒躁");
	case XING_HONG_SHI_ZI:
        return QStringLiteral("猩红十字");
    case CHONG_YING:
        return QStringLiteral("额外攻击行动");
    default:
        return "CauseString undefined";
    }
}

QString getCommandString(network::Command *cmd){
    QString msg;
    int targetID = cmd->dst_ids(0);
    switch (cmd->respond_id())
    {
    case network::RESPOND_DISCARD:
    {
        int cause = cmd->args(0);
        int howMany = cmd->args(1);
        if(cause != CAUSE_OVERLOAD){
            msg = QStringLiteral("等待") + dataInterface->getPlayerById(targetID)->getRoleName() + getCauseString(cause) + QStringLiteral("(手牌）响应");
        }
        else{
            msg = dataInterface->getPlayerById(targetID)->getRoleName()+QStringLiteral("需要弃")+QString::number(howMany)+QStringLiteral("张牌");
            msg += cmd->args(2) == 1 ? msg+QStringLiteral("(明弃)") : msg+QStringLiteral("(暗弃)");
        }
        return msg;
    }
    case network::RESPOND_DISCARD_COVER:
    {
        int cause = cmd->args(0);
        int howMany = cmd->args(1);
        if(cause != CAUSE_OVERLOAD){
            msg = QStringLiteral("等待") + dataInterface->getPlayerById(targetID)->getRoleName() + getCauseString(cause) + QStringLiteral("（盖牌）响应");
        }
        else{
            msg = dataInterface->getPlayerById(targetID)->getRoleName()+QStringLiteral("需要弃")+QString::number(howMany)+QStringLiteral("张盖牌");
        }
        return msg;
    }
    }
    return "CommandString undefined";
}


#endif // COMMON_H
