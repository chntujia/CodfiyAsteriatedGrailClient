#include "JianSheng.h"

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
