#ifndef QNRBF_TIMESPAN_H
#define QNRBF_TIMESPAN_H

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.1.1.4 TimeSpan

class QNRBF_API TimeSpan {
public:
    TimeSpan();
    explicit TimeSpan(qint64 data);

    TimeSpan(const TimeSpan &other);
    TimeSpan(TimeSpan &&other) noexcept;

    TimeSpan &operator=(const TimeSpan &other);
    TimeSpan &operator=(TimeSpan &&other) noexcept;

    qint64 ticks;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_TIMESPAN_H
