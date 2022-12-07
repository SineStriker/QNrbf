#ifndef QNRBF_BINARYMETHODRETURN_H
#define QNRBF_BINARYMETHODRETURN_H

#include "Structures/ArrayOfValueWithCode.h"
#include "Structures/StringValueWithCode.h"
#include "Structures/ValueWithCode.h"

QNRBF_BEGIN_NAMESPACE

// 2.2.3.3 BinaryMethodReturn

class QNRBF_INTERNAL BinaryMethodReturn {
public:
    BinaryMethodReturn();

    quint32 messageEnum;
    ValueWithCode returnValue;
    StringValueWithCode callContext;
    ArrayOfValueWithCode args;

    bool read(QDataStream &in);
    bool write(QDataStream &out) const;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_BINARYMETHODRETURN_H
