#ifndef QNRBF_BINARYARRAY_H
#define QNRBF_BINARYARRAY_H

#include <QVariant>

#include "Common/RemotingTypeInfo.h"
#include "Enums/BinaryArrayTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

// 2.4.3.1 BinaryArray

class QNRBF_API BinaryArray {
public:
    BinaryArray();

    qint32 objectId;
    BinaryArrayTypeEnumeration binaryArrayTypeEnum;
    qint32 rank;
    QList<qint32> lengths;
    QList<qint32> lowerBounds;

    BinaryTypeEnumeration binaryType;
    RemotingTypeInfo additionInfo;

    bool read(QDataStream &in);
};

QNRBF_END_NAMESPACE

#endif // QNRBF_BINARYARRAY_H
