#ifndef QNRBF_CLASSMEMBEROBJECT_H
#define QNRBF_CLASSMEMBEROBJECT_H

#include "BinaryObject.h"
#include "Structures/ClassInfo.h"
#include "Structures/MemberTypeInfo.h"

#include <QSharedPointer>

QNRBF_BEGIN_NAMESPACE

class QNRBF_API ClassMemberObject : public AbstractObject {
public:
    ClassInfo classInfo;
    MemberTypeInfo memberTypeInfo;
    QSharedPointer<BinaryObject> value;

    enum Type {
        User = 1,
        WithTypes = 2, // Flag
        UserWithTypes = User | WithTypes,
        System = 4,
        SystemWithTypes = System | WithTypes,
    };

    explicit ClassMemberObject(Type t) : t(t) {
    }

    inline Type type() const {
        return t;
    }

protected:
    Type t;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_CLASSMEMBEROBJECT_H
