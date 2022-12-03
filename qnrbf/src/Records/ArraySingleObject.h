#ifndef QNRBF_ARRAYSINGLEOBJECT_H
#define QNRBF_ARRAYSINGLEOBJECT_H

#include "Structures/ArrayInfo.h"

QNRBF_BEGIN_NAMESPACE

// 2.4.3.2 ArraySingleObject

class QNRBF_INTERNAL ArraySingleObject {
public:
    ArrayInfo arrayInfo;

    bool read(QDataStream &in);
};

QNRBF_END_NAMESPACE

#endif // QNRBF_ARRAYSINGLEOBJECT_H
