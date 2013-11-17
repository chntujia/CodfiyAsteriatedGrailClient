#include "KuangZhan.h"
enum CAUSE{
    XUE_YING_KUANG_DAO = 201,
    XUE_XING_PAO_XIAO = 202,
    KUANG_HUA = 203,
    XUE_YING_KUANG_DAO_USED = 204,
    SI_LIE = 205
};

KuangZhan::KuangZhan()
{
    makeConnection();
    setMyRole(this);
}
