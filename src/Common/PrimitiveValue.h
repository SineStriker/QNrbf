#ifndef QNRBF_PRIMITIVEVALUE_H
#define QNRBF_PRIMITIVEVALUE_H

#include "Enums/PrimitiveTypeEnumeration.h"
#include "Primitive/DateTime.h"
#include "Primitive/TimeSpan.h"

#include <QVariant>

QNRBF_BEGIN_NAMESPACE

// 2.2.2.1 PrimitiveValue

class QNRBF_INTERNAL PrimitiveValue {
public:
    bool toBool() const;
    quint8 toByte() const;
    QChar toChar() const;
    QString toDecimal() const;
    double toDouble() const;
    qint16 toInt16() const;
    qint32 toInt32() const;
    qint64 toInt64() const;
    qint8 toSByte() const;
    float toSingle() const;
    TimeSpan toTimeSpan() const;
    DateTime toDateTime() const;
    quint16 toUInt16() const;
    quint32 toUInt32() const;
    quint64 toUInt64() const;
    QString toString() const;

    QVariant _data;

    bool read(QDataStream &in, PrimitiveTypeEnumeration primitiveTypeEnum);
};

QNRBF_END_NAMESPACE

#endif // QNRBF_PRIMITIVEVALUE_H
