#ifndef NVWUSHEN_H
#define NVWUSHEN_H
#include "Role.h"

class NvWuShen : public Role
{
    Q_OBJECT
public:
    NvWuShen();
    void normal();
    void askForSkill(network::Command* cmd);
    void JunShenWeiGuang();
    void JunShenWeiGuang2();
    void YingLingZhaoHuan();
public slots:
    void onOkClicked();
    void onCancelClicked();
    void ZhiXuZhiYin();

};

#endif // NVWUSHEN_H
