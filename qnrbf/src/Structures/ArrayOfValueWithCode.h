#ifndef QNRBF_ARRAYOFVALUEWITHCODE_H
#define QNRBF_ARRAYOFVALUEWITHCODE_H

#include "ValueWithCode.h"

QNRBF_BEGIN_NAMESPACE

// 2.2.2.3 ArrayOfValueWithCode

class QNRBF_INTERNAL ArrayOfValueWithCode {
public:
    QList<ValueWithCode> values;

    bool read(QDataStream &in);
    bool write(QDataStream &out)const;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_ARRAYOFVALUEWITHCODE_H
