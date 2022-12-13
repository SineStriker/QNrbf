#ifndef QNRBF_DATETIME_H
#define QNRBF_DATETIME_H

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.1.1.5 DateTime

class QNRBF_API DateTime {
public:
    enum Kind {
        Unspecified,
        Utc,
        Local,
    };

    DateTime();
    explicit DateTime(quint64 data);

    DateTime(const DateTime &other);
    DateTime(DateTime &&other) noexcept;

    DateTime &operator=(const DateTime &other);
    DateTime &operator=(DateTime &&other) noexcept;

    qint64 ticks() const;
    Kind kind() const;

    quint64 _data;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_DATETIME_H
