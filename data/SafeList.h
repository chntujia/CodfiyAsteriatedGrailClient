#ifndef SAFELIST_H
#define SAFELIST_H

#include <QtCore/qlist.h>
template <class T>
class SafeList : public QList<T>
{
public:
    inline SafeList(){}
    inline ~SafeList(){}
    const T &at(int i) const{
        if(i < 0 || i >= size())
            throw 22;
        return QList<T>::at(i);
    }
    const T &operator[](int i) const{ return at(i); }
};

#endif // SAFELIST_H
