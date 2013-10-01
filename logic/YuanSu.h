#ifndef YUANSU_H
#define YUANSU_H
#include "Role.h"
class YuanSu : public Role
{
    Q_OBJECT
public:
    YuanSu();
    void normal();
    void magicAction();
    void additionalAction();
    void turnBegin();
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void YuanSuFaShu1();
    void YuanSuFaShu2();
    void YuanSuDianRan();
    void YueGuang();
private:
    Card* magicCard;
    int magicTargetID;
    bool wind,earth,ignite;
};

#endif // YUANSU_H
