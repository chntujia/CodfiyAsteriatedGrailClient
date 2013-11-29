#ifndef MOJIAN_H
#define MOJIAN_H
#include "Role.h"
class MoJian : public Role
{
    Q_OBJECT
public:
    MoJian();
    void normal();
    void askForSkill(network::Command* cmd);
    void attackAction();
    void turnBegin();
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void AnYingLiuXing();
    void XiuLuoLianZhan();
    void AnYingNingJu();
    void HeiAnZhenChan();
};

#endif // MOJIAN_H
