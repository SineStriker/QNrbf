#ifndef QNRBF_ARRAYINFO_H
#define QNRBF_ARRAYINFO_H

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.4.2.1 ArrayInfo

class QNRBF_INTERNAL ArrayInfo {
public:
    ArrayInfo();

    qint32 objectId;
    qint32 length;

    bool read(QDataStream &in);
    bool write(QDataStream &out) const;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_ARRAYINFO_H