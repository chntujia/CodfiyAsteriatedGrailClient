#ifndef MOJIAN_H
#define MOJIAN_H
#include "Role.h"
class MoJian : public Role
{
    Q_OBJECT
public:
    MoJian();
    void normal();
    void attackAction();
    void askForSkill(network::Command* cmd);
    void attacked(QString element, int hitRate,int cardId,bool canLight);
    void XiuLuoLianZhan();
    void AnYingNingJu();

public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void AnYingLiuXing();

};

#endif // MOJIAN_H
