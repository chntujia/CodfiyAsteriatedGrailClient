#ifndef LINGFU_H
#define LINGFU_H
#include "Role.h"
class LingFu : public Role
{
    Q_OBJECT
public:
    LingFu();
    void normal();    
    void BaiGuiYeXing_Expose();
    void BaiGuiYeXing_Fire();
    void BaiGuiYeXing(bool canRevert);

    void askForSkill(network::Command* cmd);
    void drop(int howMany, int cause);
    void dropCover(int howMany, int cause);
    void cardAnalyse();

public slots:
    void onOkClicked();
    void onCancelClicked();
    void LeiMing();
    void FengXing();
private:

};

#endif // LINGFU_H
