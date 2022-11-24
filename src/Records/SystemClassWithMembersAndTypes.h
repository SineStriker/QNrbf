#ifndef QNRBF_SYSTEMCLASSWITHMEMBERSANDTYPES_H
#define QNRBF_SYSTEMCLASSWITHMEMBERSANDTYPES_H

#include "Common/ClassMemberObject.h"

QNRBF_BEGIN_NAMESPACE

// 2.3.2.3 SystemClassWithMembersAndTypes

class QNRBF_API SystemClassWithMembersAndTypes : public ClassMemberObject {
public:
    SystemClassWithMembersAndTypes();

    bool read(QDataStream &in);
};

QNRBF_END_NAMESPACE

#endif // QNRBF_SYSTEMCLASSWITHMEMBERSANDTYPES_H
