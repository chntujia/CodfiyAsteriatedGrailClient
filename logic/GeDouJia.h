#ifndef GEDOUJIA_H
#define GEDOUJIA_H
#include "Role.h"
class GeDouJia : public Role
{
    Q_OBJECT
public:
    GeDouJia();
    void XuLiCangYan(int skillID);
    void NianDan();
    void BaiShiDouShen();
    void BaiShiHuanLongQuan();
    void DouShenTianQu();
    void askForSkill(network::Command* cmd);
public slots:
    void onOkClicked();
    void onCancelClicked();
    void playerAnalyse();
    void cardAnalyse();
    void buy();
    void synthetize();
    void extract();
private:
    int baiShiTargetID;
    bool baiShiFirst;
    bool baiShiUsed;
    bool cardReady;
};

#endif // GEDOUJIA_H
