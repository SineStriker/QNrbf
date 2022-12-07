#ifndef QNRBF_CLASSWITHID_H
#define QNRBF_CLASSWITHID_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.3.2.5 ClassWithId
class QNRBF_INTERNAL ClassWithId {
public:
    ClassWithId();

    qint32 objectId;
    qint32 metadataId;

    bool read(QDataStream &in);
    bool write(QDataStream &out) const;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_CLASSWITHID_H
