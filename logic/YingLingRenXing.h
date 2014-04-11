#ifndef YINGLINGRENXING_H
#define YINGLINGRENXING_H
#include "Role.h"

class YingLingRenXing : public Role
{
    Q_OBJECT
public:
    YingLingRenXing();
    void normal();
    void askForSkill(network::Command* cmd);
    void NuHuoMoWen();
    void MoWenRongHe();
    void ZhanWenSuiJi();
    void FuWenGaiZao();
    void ShuangChongHuiXiang();
public slots:
	void onOkClicked();
	void onCancelClicked();
    void cardAnalyse();
};

#endif // YINGLINGRENXING_H
