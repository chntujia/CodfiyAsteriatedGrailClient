#ifndef FENGYIN_H
#define FENGYIN_H
#include "Role.h"
class FengYin : public Role
{
    Q_OBJECT
public:
    FengYin();
    void normal();
    void additionalAction();
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void playerAnalyse();
    void FengYinFaShu();
    void WuXiShuFu();
    void FengYinPoSui();
    void onUnready();
private:
    bool usedGem;
};

#endif // FENGYIN_H
