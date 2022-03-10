#ifndef SHOULING_H
#define SHOULING_H
#include "Role.h"
class ShouLing : public Role
{
    Q_OBJECT
public:
    ShouLing();
    void ShouHunJingJie();
	void ShouFan();
	void ShouFan1();
	void NiFanHit();
	void YuHunBoot();
	void YuHunBoot1();
	void askForSkill(network::Command* cmd);
public slots:
	void cardAnalyse();
	void onOkClicked();
	void onCancelClicked();
private:
	int shoufannum = 0;

};

#endif // SHOULING_H
