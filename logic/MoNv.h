#ifndef MONV_H
#define MONV_H
#include "Role.h"

class MoNv : public Role
{
    Q_OBJECT
public:
    MoNv();
    void MoNvZhiNu();
    void TiShenWanOu();
    void MoNengFanZhuan();
    int checkFire();
    void enbleFire();
    void enbleFireAttack(QString element);
    void askForSkill(network::Command* cmd);
    void attacked(QString element,int hitRate,int cardId,bool canLight);
public slots:
    void normal();
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void CangYanFaDian();
    void TianHuoDuanKong();
    void TongKuLianJie();
};

#endif // MONV_H
