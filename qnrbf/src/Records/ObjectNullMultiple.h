#ifndef QNRBF_OBJECTNULLMULTIPLE_H
#define QNRBF_OBJECTNULLMULTIPLE_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.5.5 ObjectNullMultiple
// 2.5.6 ObjectNullMultiple256

class QNRBF_INTERNAL ObjectNullMultiple {
public:
    ObjectNullMultiple();

    qint32 nullCount;

    bool read(QDataStream &in, bool is256);
    bool write(QDataStream &out, bool is256) const;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_OBJECTNULLMULTIPLE_H
