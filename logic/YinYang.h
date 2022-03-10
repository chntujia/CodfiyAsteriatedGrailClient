#ifndef YINYANG_H
#define YINYANG_H
#include "Role.h"
class YinYang : public Role
{
    Q_OBJECT
public:
	YinYang();
    void normal();
    void magicAction();
	void YinYangZhuanHuan(int srcId,int cardId);
	void HeiAnJiLi();
	void ShiShenZhouShu(int srcId, int cardId);
	void askForSkill(network::Command* cmd);
	int sourceId;
	int atkCardId;

public slots:
	void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void ShiShenJiangLin();
    void ShengMingJieJie();
	void yinyangView();
	void normalView();
	void ShiShenon();
	void ShiShenoff();
	
signals:
	void yyskill();

private :
	bool shishenon;
	bool yinyangon;
	QString Pro;
	QString Ele;
	Button *yinyangchange;
	Button *shishenchange;
};

#endif // YINYANG_H
