#ifndef MOQIANG_H
#define MOQIANG_H
#include "Role.h"
class MoQiang : public Role
{
    Q_OBJECT
public:
    MoQiang();
    void normal();
    void AnZhiJieFang();
    void HuanYingXingChen(int flag);
    void AnZhiBiZhang();
    void QiHeiZhiQiang();
    void turnBegin();
    void askForSkill(QString skill);
    void additionalAction();
    void attacked(QString element, int hitRate);
    void moDaned(int nextID, int sourceID, int howMany);
public slots:
    void ChongYing();
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void playerAnalyse();
private:
    bool jieFangUsed;
    bool jieFangFirst;
    bool chongYingUsed;
    int huanYingFlag;
    bool cardReady;
};

#endif // MOQIANG_H
