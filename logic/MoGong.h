#ifndef MOGONG_H
#define MOGONG_H
#include "Role.h"

class MoGong : public Role
{
    Q_OBJECT
public:
    MoGong();
    void normal();
    void turnBegin();
    void MoGuanChongJi();
    void MoGuanChongJiHit();
    void DuoChongSheJi();
    void ChongNengMoYan();
    void ChongNeng();
    void MoYan();
    void ChongNengGaiPai();
    void MoYanGaiPai();
    void attackAction();
    void askForSkill(network::Command* cmd);

public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void coverCardAnalyse();
  //  void showCoverArea();
  //  void closeCoverArea();
    void LeiGuangSanShe();
private:
    bool MoGuanChongJiUsed;
    bool DuoChongSheJiUsed;
    bool ChongNengUsed;
    int  lastTarget;     //上次攻击的目标  for【多重射击】
    int  startChoice;
    int  chongnengNum;
};

#endif // MOGONG_H
