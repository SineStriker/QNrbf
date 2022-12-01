#ifndef QNRBF_CLASSMEMBEROBJECT_H
#define QNRBF_CLASSMEMBEROBJECT_H

#include "MappingObject.h"
#include "Structures/ClassInfo.h"
#include "Structures/MemberTypeInfo.h"

#include <QSharedPointer>

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL ClassMemberObject : public AbstractObject {
public:
    enum ClassType {
        User = 1,
        WithTypes = 2, // Flag
        UserWithTypes = User | WithTypes,
        System = 4,
        SystemWithTypes = System | WithTypes,
    };

    ClassMemberObject(ClassType t);

    ClassInfo classInfo;
    MemberTypeInfo memberTypeInfo;
    QSharedPointer<MappingObject> value;
    qint32 libraryId;

    inline ClassType classType() const {
        return ct;
    }

protected:
    ClassType ct;
};

using ClassRef = QSharedPointer<ClassMemberObject>;

QNRBF_END_NAMESPACE

#endif // QNRBF_CLASSMEMBEROBJECT_H
