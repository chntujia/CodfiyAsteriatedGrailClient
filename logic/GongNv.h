#ifndef GONGNV_H
#define GONGNV_H
#include "Role.h"
class GongNv : public Role
{
    Q_OBJECT
public:
    GongNv();
    void GuanChuanSheJi();
    void JingZhunSheJi();
    void normal();
    void askForSkill(network::Command* cmd);
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void ShanGuangXianJing();
    void JuJi();
private:
    bool JuJiAdditon;
};

#endif // GONGNV_H
