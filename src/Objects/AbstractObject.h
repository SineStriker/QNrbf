#ifndef QNRBF_ABSTRACTOBJECT_H
#define QNRBF_ABSTRACTOBJECT_H

#include <QDataStream>
#include <QSharedPointer>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL AbstractObject {
public:
    enum Type {
        Binary,
        ClassMember,
        Data,
        DeferredReference,
        MemberPrimitive,
        Null,
    };

    AbstractObject(Type type);
    virtual ~AbstractObject();

    inline Type type() const {
        return t;
    }

protected:
    Type t;
};

using ObjectRef = QSharedPointer<AbstractObject>;

QNRBF_END_NAMESPACE

#endif // QNRBF_ABSTRACTOBJECT_H
