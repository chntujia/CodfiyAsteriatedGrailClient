#ifndef YONGZHE_H
#define YONGZHE_H
#include "Role.h"
class YongZhe : public Role
{
    Q_OBJECT
public:
    YongZhe();
    void normal();
    void NuHou();
    void MingJingZhiShui();
    void JinDuanZhiLi();
    void SiDou();
    void turnBegin();
    void askForSkill(QString skill);
    void additionalAction();
public slots:
    void onOkClicked();
    void onCancelClicked();
    void TiaoXin();
private:
    int jinDuanZhiLi;
};


#endif // YONGZHE_H
