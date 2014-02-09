#ifndef SHENGQIANG_H
#define SHENGQIANG_H
#include "Role.h"

class ShengQiang : public Role
{
    Q_OBJECT
public:
    ShengQiang();
    void normal();
    void askForSkill(Command *cmd);
    void turnBegin();
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
    void HuiYao();
    void ChengJie();
    void ShengGuangQiYu();
    void TianQiang();
    void DiQiang();
private:
    bool ChengJieAddition,HuiYaoAddition,ShengGuangQiYuAddition;

};

#endif // SHENGQIANG_H
