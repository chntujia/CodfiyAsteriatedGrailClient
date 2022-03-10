#ifndef JINGLINGSHESHOU_H
#define JINGLINGSHESHOU_H
#include "Role.h"
class JingLingSheShou : public Role
{
    Q_OBJECT
public:
	JingLingSheShou();
	void askForSkill(network::Command* cmd);
	void YuanSuSheJi();
	void YuanSuSheJiHit(network::Command* cmd);
	void DongWuHuoBan();
	void JingLingMiYiBoot();
	void JingLingMiYi();
	void drop(int howMany, int cause);
public slots:
	void cardAnalyse();
	void onOkClicked();
	void onCancelClicked();
private:
};

#endif //JINGLINGSHESHOU_H
