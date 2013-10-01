#ifndef DIEWU_H
#define DIEWU_H
#include "Role.h"

class DieWu : public Role
{
    Q_OBJECT
public:
    DieWu();
    void normal();
    void askForSkill(QString skill);
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
    void decipher(QString command);
    void WuDong1();
    void YongHua();
    void DaoNiZhiDie1();
private:
    bool wudongmopai;
    bool diaoLingFlag;
    int daoNiZhiDieFlag;
};

#endif // DIEWU_H
