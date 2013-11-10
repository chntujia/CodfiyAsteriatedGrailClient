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
    state=10;
    gui->reset();
    handArea->setQuota(1);
    handArea->enableElement("wind");
    handArea->disableMagic();
    playerArea->setQuota(1);
    decisionArea->enable(3);
}

void JianSheng::attackAction()
{
    //若是连续技的额外行动，则只能用风系
    if(LIAN_XU_JI == chosenAction){
        LianXuJi();
    }
    else{
        Role::attackAction();
    }
}
