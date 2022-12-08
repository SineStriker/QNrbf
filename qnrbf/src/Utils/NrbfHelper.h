#ifndef QNRBFFORMAT_NRBFHELPER_H
#define QNRBFFORMAT_NRBFHELPER_H

#include <QStringList>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

namespace Helper {

    template <class T>
    QString num2hex(T num) {
        static const int print_base = 16;
        return "0x" + QString::number(num, print_base).toUpper();
    }

    template <class T>
    static void resizeList(QList<T> &list, int size) {
        if (list.size() > size) {
            list.erase(list.begin() + size, list.end());
            return;
        }
        list.reserve(size);
        for (int i = list.size(); i < size; ++i) {
            list.append(T{});
        }
    }

    QNRBF_INTERNAL QString toBackingField(const QString &key);

} // namespace Helper

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_NRBFHELPER_H
