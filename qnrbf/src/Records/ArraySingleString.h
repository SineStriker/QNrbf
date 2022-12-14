#ifndef QNRBF_ARRAYSINGLESTRING_H
#define QNRBF_ARRAYSINGLESTRING_H

#include "Structures/ArrayInfo.h"

QNRBF_BEGIN_NAMESPACE

// 2.4.3.4 ArraySingleString
class QNRBF_INTERNAL ArraySingleString {
public:
    ArrayInfo arrayInfo;

    bool read(QDataStream &in);
    bool write(QDataStream &out) const;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_ARRAYSINGLESTRING_H
