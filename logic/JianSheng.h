#ifndef JIANSHENG_H
#define JIANSHENG_H
#include "Role.h"
class JianSheng : public Role
{
    Q_OBJECT
public:
    JianSheng();
    void LieFengJi();
    void LianXuJi();
    void JianYing();
    void turnBegin();
    void askForSkill(QString skill);
    void additionalAction();
public slots:
    void onOkClicked();
private:
    bool onceUsed2;
    int JiFengJi;
};

#endif // JIANSHENG_H
