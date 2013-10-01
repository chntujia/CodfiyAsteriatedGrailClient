#ifndef QIDAO_H
#define QIDAO_H
#include "Role.h"
class QiDao  : public Role
{
    Q_OBJECT
public:
    QiDao();
    void normal();
    void magicAction();
    void QiDong();
    void FaLiChaoXi();
    void askForSkill(QString skill);
    void additionalAction();
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void GuangHuiXinYang();
    void QiHeiXinYang();
    void WeiLiCiFu();
    void XunJieCiFu();

};

#endif // QIDAO_H
