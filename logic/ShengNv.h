#ifndef SHENGNV_H
#define SHENGNV_H
#include "Role.h"
class ShengNv : public Role
{
    Q_OBJECT
public:
    ShengNv();
    void BingShuangDaoYan();
    void LianMin();
    void normal();
    void attackOrMagic();
    void turnBegin();
    void askForSkill(QString skill);
    void additionalAction();
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void ZhiLiaoShu();
    void ZhiYuZhiGuang();
    void ShengLiao();
    void ShengLiao2();
    void ShengLiao3();    
private:
    bool ShengLiaoAddition;
};
#endif // SHENGNV_H
