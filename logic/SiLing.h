#ifndef SILING_H
#define SILING_H
#include "Role.h"

class SiLing : public Role{
    Q_OBJECT
public:
    SiLing();
    void normal();
    void askForSkill(QString skill);
    void BuXiu();
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void WenYi();
    void SiWangZhiChu();
    void MuBeiYunLuo();
};

#endif // SILING_H
