#ifndef QNRBF_SERIALIZATIONHEADER_H
#define QNRBF_SERIALIZATIONHEADER_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.6.1 SerializationHeaderRecord

class QNRBF_API SerializationHeader {
public:
    SerializationHeader();

    qint32 rootId;
    qint32 headerId;
    qint32 majorVersion;
    qint32 minorVersion;

    bool read(QDataStream &in);
};

QNRBF_END_NAMESPACE

#endif // QNRBF_SERIALIZATIONHEADER_H
