#ifndef QNRBF_CLASSWITHMEMBERS_H
#define QNRBF_CLASSWITHMEMBERS_H

#include "Structures/ClassInfo.h"

QNRBF_BEGIN_NAMESPACE

// 2.3.2.2 ClassWithMembers

class QNRBF_INTERNAL ClassWithMembers {
public:
    ClassWithMembers();

    ClassInfo classInfo;
    qint32 libraryId;

    bool read(QDataStream &in);
    bool write(QDataStream &out) const;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_CLASSWITHMEMBERS_H
