#ifndef KUANGZHAN_H
#define KUANGZHAN_H
#include "Role.h"
class KuangZhan : public Role
{
    Q_OBJECT
public:
    KuangZhan();
    void SiLie();
    void XueXingPaoXiao();
    void XueYingKuangDao();
    void askForSkill(QString skill);
};

#endif // KUANGZHAN_H
