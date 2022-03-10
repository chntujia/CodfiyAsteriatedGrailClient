#ifndef YUENV_H
#define YUENV_H
#include "Role.h"

class YueNv : public Role
{
    Q_OBJECT
public:
    YueNv();
    void normal();
    void askForSkill(network::Command* cmd);
    void XinYueBiHu();
    void MeiDuSha(int cardId);
	void MeiDuSha_2();
    void YueZhiLunHui();
	void YueZhiLunHui_2();
    void YueDu();
	void AnYueZhan();
	void CangBaiZhiYue_2();
public slots:
	void CangBaiZhiYue();
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
	void coverCardAnalyse();
private:
    int meidusha_card;
	int meidusha_anyue;
};

#endif // YUENV_H
