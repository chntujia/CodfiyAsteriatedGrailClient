#ifndef MOGONG_H
#define MOGONG_H
#include "Role.h"

class MoGong : public Role
{
    Q_OBJECT
public:
    MoGong();
    void normal();
    void askForSkill(QString skill);
    void turnBegin();
    void additionalAction();
    void MoGuanChongJiHit();
    void DuoChongSheJi();
    void ChongNengMoYan1();
    void ChongNengMoYan2();
    void ChongNengGaiPai();
    void MoYanGaiPai();
    void MoGuanChongJi();
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void coverCardAnalyse();    
    void LeiGuangSanShe();
private:
    bool MoGuanChongJiUsed;
    bool DuoChongSheJiUsed;
    bool ChongNengUsed;
    int lastTarget;
    int startChoice;
    int chongnengNum;
};

#endif // MOGONG_H
