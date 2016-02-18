#ifndef SPMODAO_H
#define SPMODAO_H
#include "Role.h"
class spMoDao : public Role
{
    Q_OBJECT
public:
    spMoDao();
    void normal();
    void FaLiHuDun();
    void turnBegin();
    void moDaned(int nextID, int sourceID, int howMany);
    void additionalAction();
    void askForSkill(network::Command* cmd);
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void MoDanRongHe();
    void spMoBaoChongJi();
    void HuiMeiFengBao();
private:
    int nextClockwise;
    int moDanHarm;
    bool firstMoDan;
};

#endif // SPMODAO_H
