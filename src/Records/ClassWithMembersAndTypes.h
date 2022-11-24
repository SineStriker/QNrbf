#ifndef QNRBF_CLASSWITHMEMBERSANDTYPES_H
#define QNRBF_CLASSWITHMEMBERSANDTYPES_H

#include "Common/ClassMemberObject.h"

QNRBF_BEGIN_NAMESPACE

// 2.3.2.1 ClassWithMembersAndTypes

class QNRBF_API ClassWithMembersAndTypes : public ClassMemberObject {
public:
    ClassWithMembersAndTypes();

    qint32 libraryId;

    bool read(QDataStream &in);
};

QNRBF_END_NAMESPACE

#endif // QNRBF_CLASSWITHMEMBERSANDTYPES_H
