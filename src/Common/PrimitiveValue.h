#ifndef QNRBF_PRIMITIVEVALUE_H
#define QNRBF_PRIMITIVEVALUE_H

#include "Enums/PrimitiveTypeEnumeration.h"
#include "Primitive/DateTime.h"
#include "Primitive/Decimal.h"
#include "Primitive/TimeSpan.h"

#include <QSharedDataPointer>

QNRBF_BEGIN_NAMESPACE

class PrimitiveValueData;

class QNRBF_INTERNAL PrimitiveValue {
public:
    PrimitiveValue();
    PrimitiveValue(const PrimitiveValue &other);
    PrimitiveValue &operator=(const PrimitiveValue &other);
    ~PrimitiveValue();

    PrimitiveValue(PrimitiveTypeEnumeration nullType);
    PrimitiveValue(bool b);
    PrimitiveValue(quint8 uc);
    PrimitiveValue(const QChar &ch);
    PrimitiveValue(const Decimal &dec);
    PrimitiveValue(double d);
    PrimitiveValue(qint16 s);
    PrimitiveValue(qint32 i);
    PrimitiveValue(qint64 l);
    PrimitiveValue(qint8 c);
    PrimitiveValue(float f);
    PrimitiveValue(const TimeSpan &ts);
    PrimitiveValue(const DateTime &dt);
    PrimitiveValue(quint16 us);
    PrimitiveValue(quint32 u);
    PrimitiveValue(quint64 ul);
    PrimitiveValue(const QString &str);

    bool isValid() const;
    PrimitiveTypeEnumeration type() const;

    bool toBool() const;
    quint8 toByte() const;
    QChar toChar() const;
    Decimal toDecimal() const;
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

    bool read(QDataStream &in);
    bool read(QDataStream &in, PrimitiveTypeEnumeration primitiveTypeEnum);

protected:
    QSharedDataPointer<PrimitiveValueData> d;
};

QNRBF_END_NAMESPACE

#endif // QNRBF_PRIMITIVEVALUE_H
