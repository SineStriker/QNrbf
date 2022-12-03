#ifndef QNRBFFORMAT_NRBFREGISTRY_H
#define QNRBFFORMAT_NRBFREGISTRY_H

#include <QDataStream>
#include <QHash>
#include <QDebug>

#include "Common/PrimitiveValue.h"
#include "Common/PrimitiveValueArray.h"
#include "Objects/AbstractObject.h"
#include "Objects/ClassMemberObject.h"
#include "Records/SerializationHeader.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL NrbfRegistry {
public:
    NrbfRegistry();

    void reset();
    ObjectRef findReference(qint32 id) const;

    // Registry
    QHash<qint32, ObjectRef> objectsById;
    QHash<qint32, ClassRef> classesById;
    QHash<qint32, QString> libraries;

    // Header
    QSharedPointer<SerializationHeader> header;

public:
    static QString toBackingField(const QString &key);

public:
    using ObjectMap = QMap<QString, ObjectRef>;

    // Searching utils
    bool findString(const ObjectMap &objMap, const QString &key, QString &out) const;

    bool findPrimitive(const ObjectMap &objMap, const QString &key, PrimitiveValue &out) const;

    bool findObject(const ObjectMap &objMap, const QString &key, MappingRef &out) const;

    template <class T>
    bool findPrimitive(const ObjectMap &objMap, const QString &key, PrimitiveTypeEnumeration type,
                       T &out) const {
        PrimitiveValue val;
        if (!findPrimitive(objMap, key, val)) {
            return false;
        }
        if (val.type() == PrimitiveTypeEnumeration::None) {
            out = T{};
            return true;
        }
        if (val.type() != type) {
            return false;
        }
        out = *reinterpret_cast<const T *>(val.constData());
        return true;
    }

    bool findStringList(const ObjectMap &objMap, const QString &key, QStringList &out) const;

    bool findPrimitiveList(const ObjectMap &objMap, const QString &key,
                           PrimitiveValueArray &out) const;

    template <class T>
    bool findPrimitiveList(const ObjectMap &objMap, const QString &key,
                           PrimitiveTypeEnumeration type, QList<T> &out) const {
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

    bool findObjectList(const ObjectMap &objMap, const QString &key, QList<ObjectRef> &out) const;

    ObjectRef resolveReference(ObjectRef obj) const;

protected:
    ObjectRef findRealObject(const ObjectMap &objMap, const QString &key) const;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_NRBFREGISTRY_H
