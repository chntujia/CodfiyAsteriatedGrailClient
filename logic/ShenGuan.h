#ifndef SHENGUAN_H
#define SHENGUAN_H
#include "Role.h"

class ShenGuan: public Role{
    Q_OBJECT
public:
    ShenGuan();
    void normal();
    void askForSkill(network::Command* cmd);
    void ShuiZhiShenLi2();
    void ShenShengQiYue1();
    void ShenShengQiYue2();
    void ShenShengLingYu2();
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void ShenShengQiFu();
    void ShuiZhiShenLi1();
    void ShenShengLingYu1();
private:
    int lingYu;
};

#endif // SHENGUAN_H
