#ifndef LINGHUN_H
#define LINGHUN_H
#include "Role.h"
class LingHun : public Role
{
    Q_OBJECT
public:
    LingHun();
    void normal();
    void LingHunZhuanHuan();
    void LingHunLianJie(int harmPoint=0);
    void LingHunZengFu();
    void askForSkill(QString skill);
public slots:
    void LingHunZhaoHuan();
    void LingHunJingXiang();
    void LingHunZhenBao();
    void LingHunCiYu();
    void cardAnalyse();
    void onOkClicked();
    void onCancelClicked();
private:
    bool lianJieUsed;
};
#endif // LINGHUN_H
