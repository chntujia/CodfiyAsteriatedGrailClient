#include "JianSheng.h"
enum CAUSE{
    JI_FENG_JI = 101,
    LIE_FENG_JI = 102,
    LIAN_XU_JI = 103,
    SHENG_JIAN = 104,
    JIAN_YING = 105
};

JianSheng::JianSheng()
{
    makeConnection();
    setMyRole(this);
}

void JianSheng::LianXuJi()
{
    //借用基类的额外攻击行动状态
    handArea->disableElementExcept("wind");
}

void JianSheng::attackAction()
{
    Role::attackAction();

    //若是连续技的额外行动，则只能用风系
    if(LIAN_XU_JI == chosenAction){
        LianXuJi();
    }
}
