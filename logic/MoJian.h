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
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void AnYingLiuXing();
    void XiuLuoLianZhan();
};

#endif // MOJIAN_H
