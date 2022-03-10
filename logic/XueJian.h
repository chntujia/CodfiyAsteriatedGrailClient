#ifndef XUEJIAN_H
#define XUEJIAN_H
#include "Role.h"
class XueJian : public Role
{
    Q_OBJECT
public:
    XueJian();
    void normal();
    void magicAction();
	void askForSkill(network::Command* cmd);
public slots:
	void cardAnalyse();
    void onOkClicked();
    void onCancelClicked();
    void XueRanQiangWei1();
    void XueRanQiangWei2();
    void XueQiPingZhang();
    void XueQiangWeiBoot();
	void XueQiangWeiBoot2();
private:
    int XueRanTargetID=0;
};

#endif // XUEJIAN_H
