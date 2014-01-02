#ifndef JIANDI_H
#define JIANDI_H
#include "Role.h"
class JianDi : public Role
{
    Q_OBJECT
public:
    JianDi();
    void JianHunShouHu();
    void JianQiZhan(int targetID);
    void TianShiZhiHun();
    void BuQiYiZhi();
    void EMoZhiHun();
    void askForSkill(network::Command* cmd);
    void coverCardAnalyse();
public slots:
    void onOkClicked();
    void onCancelClicked();
private:
    int shouhuID;
    int jianqiID;
    QString jianqiPoint;
};

#endif // JIANDI_H
