#ifndef QNRBF_CLASSWITHMEMBERS_H
#define QNRBF_CLASSWITHMEMBERS_H

#include "Structures/ClassInfo.h"

QNRBF_BEGIN_NAMESPACE

// 2.3.2.2 ClassWithMembers

class QNRBF_API ClassWithMembers {
public:
    ClassWithMembers();

    ClassInfo classInfo;
    qint32 libraryId;

    bool read(QDataStream &in);
};

QNRBF_END_NAMESPACE

#endif // QNRBF_CLASSWITHMEMBERS_H
