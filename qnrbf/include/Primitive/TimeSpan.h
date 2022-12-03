#ifndef QNRBF_TIMESPAN_H
#define QNRBF_TIMESPAN_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.1.1.4 TimeSpan

class QNRBF_API TimeSpan {
public:
    TimeSpan();
    explicit TimeSpan(quint64 data);

    TimeSpan(const TimeSpan &other);
    TimeSpan(TimeSpan &&other) noexcept;

    TimeSpan &operator=(const TimeSpan &other);
    TimeSpan &operator=(TimeSpan &&other) noexcept;

    quint64 _data;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_TIMESPAN_H
