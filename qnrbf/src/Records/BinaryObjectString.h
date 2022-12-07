#ifndef QNRBF_BINARYOBJECTSTRING_H
#define QNRBF_BINARYOBJECTSTRING_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.5.7 BinaryObjectString
class QNRBF_INTERNAL BinaryObjectString {
public:
    BinaryObjectString();

    qint32 objectId;
    QString value;

    bool read(QDataStream &in);
    bool write(QDataStream &out) const;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_BINARYOBJECTSTRING_H
