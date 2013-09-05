#ifndef LINGFU_H
#define LINGFU_H
#include "Role.h"
class LingFu : public Role
{
    Q_OBJECT
public:
    LingFu();
    void normal();
    void baiGuiYeXing();
    void baiGuiYeXing2();
    void baiGuiYeXing3(bool show);
    void baiGuiYeXing4();
    void nianZhou();
    void lingLiBengJie();
    void askForSkill(QString skill);
    void cardAnalyse();
    void coverCardAnalyse();
    //void playerAnalyse();
public slots:
    void onOkClicked();
    void onCancelClicked();
    void leiMing();
    void fengXing();
    void decipher(QString command);
private:

};

#endif // LINGFU_H
