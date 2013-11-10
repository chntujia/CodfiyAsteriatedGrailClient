#ifndef FENGYIN_H
#define FENGYIN_H
#include "Role.h"
class FengYin : public Role
{
    Q_OBJECT
public:
    FengYin();
    void normal();
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
    int getMapping(QString element);
};

#endif // FENGYIN_H
