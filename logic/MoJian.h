#ifndef MOJIAN_H
#define MOJIAN_H
#include "Role.h"
class MoJian : public Role
{
    Q_OBJECT
public:
    MoJian();
    void normal();
    void XiuLuoLianZhan();
    void AnYingNingJu();
    void HeiAnZhenChan();
    void askForSkill(QString skill);
    void additionalAction();
    void attacked(QString element, int hitRate);
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void AnYingLiuXing();
};

#endif // MOJIAN_H
