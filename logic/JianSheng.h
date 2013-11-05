#ifndef JIANSHENG_H
#define JIANSHENG_H
#include "Role.h"
class JianSheng : public Role
{
    Q_OBJECT
public:
    JianSheng();
    void LianXuJi();
    void attackAction();
};

#endif // JIANSHENG_H
