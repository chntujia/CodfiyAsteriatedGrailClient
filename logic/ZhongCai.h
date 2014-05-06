#ifndef ZHONGCAI_H
#define ZHONGCAI_H
#include "Role.h"
class ZhongCai : public Role
{
    Q_OBJECT
public:
    ZhongCai();
    void normal();
    void attackAction();
    void YiShiZhongDuan();
    void ZhongCaiYiShi();
    void askForSkill(network::Command* cmd);
public slots:
    void MoRiShenPan();
    void PanJueTianPing();
    void onOkClicked();
    void onCancelClicked();
private:
};

#endif // ZHONGCAI_H
