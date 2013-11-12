#ifndef MODAO_H
#define MODAO_H
#include "Role.h"
class MoDao : public Role
{
    Q_OBJECT
public:
    MoDao();
    void normal();
    void turnBegin();
    void moDaned(int nextID, int sourceID, int howMany);
    void additionalAction();
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void MoDanRongHe();
    void MoBaoChongJi();
    void HuiMeiFengBao();
private:
    int nextClockwise;
    int moDanHarm;
    bool firstMoDan;
};

#endif // MODAO_H
