#ifndef XIANZHE_H
#define XIANZHE_H
#include "Role.h"
class XianZhe : public Role
{
    Q_OBJECT
public:
    XianZhe();
    void normal();
    void FaShuFanTan1();
    void FaShuFanTan2();
    void askForSkill(network::Command* cmd);
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void playerAnalyse();
    void MoDaoFaDian();
    void ShengJieFaDian();
private:
    bool cardReady;
};

#endif // XIANZHE_H
