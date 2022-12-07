#ifndef QNRBF_MEMBERREFERENCE_H
#define QNRBF_MEMBERREFERENCE_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.5.3 MemberReference

class QNRBF_INTERNAL MemberReference {
public:
    MemberReference();

    qint32 idRef;

    bool read(QDataStream &in);
    bool write(QDataStream &out) const;
};

QNRBF_END_NAMESPACE


#endif // QNRBF_MEMBERREFERENCE_H
