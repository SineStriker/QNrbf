#ifndef QNRBFFORMAT_NRBFFINDER_H
#define QNRBFFORMAT_NRBFFINDER_H

#include <QMap>

#include "Common/PrimitiveValue.h"
#include "Common/PrimitiveValueArray.h"
#include "Enums/PrimitiveTypeEnumeration.h"
#include "Objects/AbstractObject.h"
#include "Objects/MappingObject.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL NrbfFinder {
public:
    using ObjectMap = QMap<QString, ObjectRef>;

    static QString toBackingField(const QString &key);

    static bool findString(const ObjectMap &objMap, const QString &key, QString &out);

    static bool findPrimitive(const ObjectMap &objMap, const QString &key, PrimitiveValue &out);

    static bool findObject(const ObjectMap &objMap, const QString &key, MappingRef &out);

    template <class T>
    static bool findPrimitive(const ObjectMap &objMap, const QString &key,
                              PrimitiveTypeEnumeration type, T &out) {
        PrimitiveValue val;
        if (!findPrimitive(objMap, key, val)) {
            return false;
        }
        if (val.type() == PrimitiveTypeEnumeration::None) {
            out = T{};
            return false;
        }
        if (val.type() != type) {
            return false;
        }
        out = *reinterpret_cast<const T *>(val.constData());
        return true;
    }

    static bool findStringList(const ObjectMap &objMap, const QString &key, QStringList &out);

    static bool findPrimitiveList(const ObjectMap &objMap, const QString &key,
                                  PrimitiveValueArray &out);

    template <class T>
    static bool findPrimitiveList(const ObjectMap &objMap, const QString &key,
                                  PrimitiveTypeEnumeration type, QList<T> &out) {
        PrimitiveValueArray val;
        if (!findPrimitiveList(objMap, key, val)) {
            return false;
        }
        if (val.type() == PrimitiveTypeEnumeration::None) {
            out.clear();
            return true;
        }
        if (val.type() != type) {
            return false;
        }
        const T *ptr = reinterpret_cast<const T *>(val.constData());
        out.clear();
        out.reserve(val.size());
        for (int i = 0; i < val.size(); ++i) {
            out.push_back(ptr[i]);
        }
        return true;
    }

    static bool findObjectList(const ObjectMap &objMap, const QString &key, QList<ObjectRef> &out);
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_NRBFFINDER_H
