#ifndef WUNV_H
#define WUNV_H
#include "Role.h"

class WuNv : public Role
{
    Q_OBJECT
public:
    WuNv();
    void normal();
    void askForSkill(Command *cmd);
    void XueZhiAiShang();
    void XueZhiAiShangZhuanYi();
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void TongShengGongSi();
    void NiLiu();
    void XueZhiBeiMing();
    void XueZhiZuZhou();
    void XueZhiZuZhouQiPai();
private:
    int tongShengID;
};

#endif // WUNV_H
