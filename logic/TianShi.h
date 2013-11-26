#ifndef TIANSHI_H
#define TIANSHI_H
#include "Role.h"
class TianShi : public Role
{
    Q_OBJECT
public:
    TianShi();
    void TianShiZhiGe1();
    void TianShiZhiGe2();
    void ShenZhiBiHu(int reduce);
    void TianShiJiBan();
    void normal();
    void askForSkill(network::Command* cmd);
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void playerAnalyse();
    void onUnready();
    void FengZhiJieJin();
    void TianShiZhuFu();
    void TianShiZhiQiang();

private:
    bool usedGem;
};

#endif // TIANSHI_H
