#ifndef QNRBF_BINARYMETHODCALL_H
#define QNRBF_BINARYMETHODCALL_H

#include "Structures/ArrayOfValueWithCode.h"
#include "Structures/StringValueWithCode.h"

QNRBF_BEGIN_NAMESPACE

// 2.2.3.1 BinaryMethodCall

class QNRBF_API BinaryMethodCall {
public:
    BinaryMethodCall();

    quint32 messageEnum;
    StringValueWithCode methodName;
    StringValueWithCode typeName;
    StringValueWithCode callContext;
    ArrayOfValueWithCode args;

    bool read(QDataStream &in);
};

QNRBF_END_NAMESPACE

#endif // QNRBF_BINARYMETHODCALL_H
