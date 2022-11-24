#ifndef __ARRAYINFO_H__
#define __ARRAYINFO_H__

#include "qnrbf_global.h"

#include <QDataStream>

QNRBF_BEGIN_NAMESPACE

// 2.4.2.1 ArrayInfo

class QNRBF_API ArrayInfo {
public:
    ArrayInfo();

    qint32 objectId;
    qint32 length;

    bool read(QDataStream &in);
};

QNRBF_END_NAMESPACE

Q_DECLARE_METATYPE(QNrbf::ArrayInfo);

#endif // __ARRAYINFO_H__