#ifndef SHENGGONG_H
#define SHENGGONG_H
#include "Role.h"

class ShengGong : public Role
{
    Q_OBJECT
public:
    ShengGong();
    void normal();
	void magicAction();
    void askForSkill(Command *cmd);
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void ShengXieJuBao();
	void ShengXieMiss();
    void ShengHuangMoShi();
    void ShengGuangBaoLie();
	void MoPai();
	void QiPai();
	void ShengGuangBaoLie2();
    void ShengLiuXingAtk();
	void ZiDongTianChong();
	void HuiGuangPao();
private:
	bool shengxie;
	int zhiliao;
	int moqi = 1;
	Button *mopai;
	Button *qipai;

};

#endif // SHENGGONG_H
