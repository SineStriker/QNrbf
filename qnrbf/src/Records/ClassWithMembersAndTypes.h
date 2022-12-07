#ifndef QNRBF_CLASSWITHMEMBERSANDTYPES_H
#define QNRBF_CLASSWITHMEMBERSANDTYPES_H

#include "Structures/ClassInfo.h"
#include "Structures/MemberTypeInfo.h"

QNRBF_BEGIN_NAMESPACE

// 2.3.2.1 ClassWithMembersAndTypes

class QNRBF_INTERNAL ClassWithMembersAndTypes {
public:
    ClassWithMembersAndTypes();

    ClassInfo classInfo;
    MemberTypeInfo memberTypeInfo;
    qint32 libraryId;

    bool read(QDataStream &in);
    bool write(QDataStream &out) const;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_CLASSWITHMEMBERSANDTYPES_H
