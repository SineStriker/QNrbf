#ifndef QNRBF_DATETIME_H
#define QNRBF_DATETIME_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.1.1.5 DateTime

class QNRBF_INTERNAL DateTime {
public:
    DateTime();
    explicit DateTime(quint64 data);

    DateTime(const DateTime &other);
    DateTime(DateTime &&other) noexcept;

    DateTime &operator=(const DateTime &other);
    DateTime &operator=(DateTime &&other) noexcept;

    int tick() const;
    int kind() const;

    quint64 _data;

    bool read(QDataStream &in);
};

QNRBF_END_NAMESPACE

#endif // QNRBF_DATETIME_H
