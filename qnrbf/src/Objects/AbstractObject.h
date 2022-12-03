#ifndef QNRBF_ABSTRACTOBJECT_H
#define QNRBF_ABSTRACTOBJECT_H

#include <QDataStream>
#include <QSet>
#include <QSharedPointer>

#include "qnrbf_global.h"

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

    AbstractObject(Type type);
    virtual ~AbstractObject();

    inline Type type() const {
        return t;
    }

    int id;

protected:
    Type t;

private:
    QSet<AbstractObject *> insts;
};

using ObjectRef = QSharedPointer<AbstractObject>;

QNRBF_END_NAMESPACE

#endif // QNRBF_ABSTRACTOBJECT_H
