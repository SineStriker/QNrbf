#ifndef QNRBF_SYSTEMCLASSWITHMEMBERSANDTYPES_H
#define QNRBF_SYSTEMCLASSWITHMEMBERSANDTYPES_H

#include "Structures/ClassInfo.h"
#include "Structures/MemberTypeInfo.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_API SystemClassWithMembersAndTypes {
public:
    ClassInfo classInfo;
    MemberTypeInfo memberTypeInfo;

    bool read(QDataStream &in);
};

QNRBF_END_NAMESPACE

#endif // QNRBF_SYSTEMCLASSWITHMEMBERSANDTYPES_H
