#ifndef HONGLIAN_H
#define HONGLIAN_H
#include "Role.h"

class HongLian : public Role
{
    Q_OBJECT
public:
    HongLian();
    void normal();
    void attackOrMagic();
    void askForSkill(network::Command* cmd);
    void XueXingDaoYan();
    void ShaLuShengYan();
    void XueXingDaoYan1();
    void XueXingDaoYan2();
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void XingHongShiZi();
private:
    int dst[2];
    int cross[2];
};

#endif // HONGLIAN_H
