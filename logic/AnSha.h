#ifndef ANSHA_H
#define ANSHA_H
#include "Role.h"
class AnSha : public Role
{
    Q_OBJECT
public:
    AnSha();
    void ShuiYing();
    void QianXing();
    void askForSkill(network::Command* cmd);
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
};

#endif // ANSHA_H
