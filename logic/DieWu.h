#ifndef DIEWU_H
#define DIEWU_H
#include "Role.h"

class DieWu : public Role
{
    Q_OBJECT
public:
    DieWu();
    void normal();
    void askForSkill(Command* cmd);
    void WuDong2();
    void DuFen();
    void ChaoSheng();
    void JingHuaShuiYue();
    void DiaoLing();
    void DaoNiZhiDie2();
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void coverCardAnalyse();
    void WuDong1();
    void YongHua();
    void DaoNiZhiDie1();
private:
    bool wudongmopai;
    int daoNiZhiDieFlag;
};

#endif // DIEWU_H
