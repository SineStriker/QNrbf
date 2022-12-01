#ifndef QNRBFFORMAT_DECIMAL_H
#define QNRBFFORMAT_DECIMAL_H

#include <QDataStream>

#include "qnrbf_global.h"

QNRBF_BEGIN_NAMESPACE

// 2.1.1.7 Decimal

class QNRBF_API Decimal {
public:
    Decimal();
    explicit Decimal(const QString &data);
    explicit Decimal(QString &&data) noexcept;

    Decimal(const Decimal &other);
    Decimal(Decimal &&other) noexcept;

    Decimal &operator=(const Decimal &other);
    Decimal &operator=(Decimal &&other) noexcept;

    QString _data;

protected:
    friend class QNrbfParserPrivate;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_DECIMAL_H
