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
    void askForSkill(QString skill);
public slots:
    void onOkClicked();
    void onCancelClicked();
    void cardAnalyse();
};

#endif // ANSHA_H
