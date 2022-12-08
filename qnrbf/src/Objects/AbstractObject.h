#ifndef QNRBF_ABSTRACTOBJECT_H
#define QNRBF_ABSTRACTOBJECT_H

#include <QDataStream>
#include <QSet>
#include <QSharedPointer>

#include "qnrbf_global.h"

#define DISABLE_DEBUG_OBJECT_COLLECT

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL AbstractObject {
public:
    enum Type {
        Mapping,
        ClassMember,
        DeferredReference,
        Primitive,
        String,
        Null,
        PrimitiveList,
        StringList,
        ObjectList,
    };

    explicit AbstractObject(Type type);
    virtual ~AbstractObject();

    inline Type type() const {
        return t;
    }

    int id;

protected:
    Type t;

#ifndef DISABLE_DEBUG_OBJECT_COLLECT
public:
    static QSet<AbstractObject *> insts;
#endif
};

using ObjectRef = QSharedPointer<AbstractObject>;

QNRBF_END_NAMESPACE

#endif // QNRBF_ABSTRACTOBJECT_H
