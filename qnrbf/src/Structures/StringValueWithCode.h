#ifndef QNRBF_STRINGVALUEWITHCODE_H
#define QNRBF_STRINGVALUEWITHCODE_H

#include "Enums/PrimitiveTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

// 2.2.2.2 StringValueWithCode

class QNRBF_INTERNAL StringValueWithCode {
public:
    StringValueWithCode();

    PrimitiveTypeEnumeration primitiveTypeEnum;
    QString stringValue;

    bool read(QDataStream &in);
    bool write(QDataStream &out)const;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_STRINGVALUEWITHCODE_H
