#ifndef QNRBFFORMAT_LISTUTILS_H
#define QNRBFFORMAT_LISTUTILS_H

#include <QList>

namespace ListUtils {

    template <class T, class Selector>
    QList<T> Where(const QList<T> &list, Selector selector) {
        QList<T> res;
        for (const auto &item : qAsConst(list)) {
            if (selector(item)) {
                res.append(item);
            }
        }
        return res;
    }

    template <class T, class Mapper>
    QList<T> Select(const QList<T> &list, Mapper mapper) {
        QList<T> res;
        res.reserve(list.size());
        for (const auto &item : qAsConst(list)) {
            res.append(mapper(item));
        }
        return res;
    }

    template <class T, class Validator>
    bool Any(const QList<T> &list, Validator validator) {
        for (const auto &item : qAsConst(list)) {
            if (validator(item)) {
                return true;
            }
        }
        return false;
    }

}; // namespace ListUtils


#endif // QNRBFFORMAT_LISTUTILS_H
