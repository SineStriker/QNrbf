#ifndef __ARRAYINFO_H__
#define __ARRAYINFO_H__

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.4.2.1 ArrayInfo

class QNRBF_INTERNAL ArrayInfo {
public:
    ArrayInfo();

    qint32 objectId;
    qint32 length;

    bool read(QDataStream &in);
};

QNRBF_END_NAMESPACE

#endif // __ARRAYINFO_H__