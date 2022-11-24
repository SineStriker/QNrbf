#ifndef QNRBF_TIMESPAN_H
#define QNRBF_TIMESPAN_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.1.1.4 TimeSpan

class QNRBF_API TimeSpan {
public:
    TimeSpan();

    quint64 _data;

    bool Read(QDataStream &in);
};

QNRBF_END_NAMESPACE

Q_DECLARE_METATYPE(QNrbf::TimeSpan);

#endif // QNRBF_TIMESPAN_H
