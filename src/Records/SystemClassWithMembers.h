#ifndef QNRBF_SYSTEMCLASSWITHMEMBERS_H
#define QNRBF_SYSTEMCLASSWITHMEMBERS_H

#include "Structures/ClassInfo.h"

QNRBF_BEGIN_NAMESPACE

// 2.3.2.4 SystemClassWithMembers

class QNRBF_API SystemClassWithMembers {
public:
    SystemClassWithMembers();

    ClassInfo classInfo;

    bool read(QDataStream &in);
};

QNRBF_END_NAMESPACE

#endif // QNRBF_SYSTEMCLASSWITHMEMBERS_H
