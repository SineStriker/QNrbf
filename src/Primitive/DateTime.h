#ifndef QNRBF_DATETIME_H
#define QNRBF_DATETIME_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.1.1.5 DateTime

class QNRBF_API DateTime {
public:
    DateTime();

    int tick() const;
    int kind() const;

    quint64 _data;

    bool Read(QDataStream &in);
};

QNRBF_END_NAMESPACE

Q_DECLARE_METATYPE(QNrbf::DateTime);

#endif // QNRBF_DATETIME_H
