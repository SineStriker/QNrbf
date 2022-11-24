#ifndef QNRBF_CLASSSERIALIZATION_H
#define QNRBF_CLASSSERIALIZATION_H

#include "Structures/ClassInfo.h"
#include "Structures/MemberTypeInfo.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_API ClassSerialization {
public:
    ClassInfo classInfo;
    MemberTypeInfo memberTypeInfo;
    // BinaryObject value ;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_CLASSSERIALIZATION_H
