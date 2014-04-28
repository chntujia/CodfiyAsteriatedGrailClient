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
    void HuanYingXingChen();
	void attacked(QString element, int hitRate);
    void AnZhiBiZhang();
    void QiHeiZhiQiang();
    void turnBegin();
    void askForSkill(Command* cmd);

public slots:
    void ChongYing();
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void playerAnalyse();
private:
    bool jieFangUsed;     //for【充盈】button
    bool jieFangFirst;
    bool cardReady;
};

#endif // MOQIANG_H
