#ifndef QNRBFFORMAT_PrimitiveValueArrayARRAY_H
#define QNRBFFORMAT_PrimitiveValueArrayARRAY_H

#include "Enums/PrimitiveTypeEnumeration.h"
#include "Primitive/DateTime.h"
#include "Primitive/Decimal.h"
#include "Primitive/TimeSpan.h"

#include <QSharedDataPointer>

QNRBF_BEGIN_NAMESPACE

class PrimitiveValueArrayData;

class QNRBF_INTERNAL PrimitiveValueArray {
public:
    PrimitiveValueArray();
    PrimitiveValueArray(const PrimitiveValueArray &other);
    PrimitiveValueArray &operator=(const PrimitiveValueArray &other);
    ~PrimitiveValueArray();

    PrimitiveValueArray(PrimitiveTypeEnumeration nullType, int size);
    PrimitiveValueArray(const QList<bool> &bs);
    PrimitiveValueArray(const QList<quint8> &ucs);
    PrimitiveValueArray(const QList<QChar> &chs);
    PrimitiveValueArray(const QList<Decimal> &decs);
    PrimitiveValueArray(const QList<double> &ds);
    PrimitiveValueArray(const QList<qint16> &ss);
    PrimitiveValueArray(const QList<qint32> &is);
    PrimitiveValueArray(const QList<qint64> &ls);
    PrimitiveValueArray(const QList<qint8> &cs);
    PrimitiveValueArray(const QList<float> &fs);
    PrimitiveValueArray(const QList<TimeSpan> &tss);
    PrimitiveValueArray(const QList<DateTime> &dts);
    PrimitiveValueArray(const QList<quint16> &uss);
    PrimitiveValueArray(const QList<quint32> &us);
    PrimitiveValueArray(const QList<quint64> &uls);
    PrimitiveValueArray(const QStringList &strs);

    bool isValid() const;
    PrimitiveTypeEnumeration type() const;
    int size() const;

    QList<bool> toBoolList() const;
    QList<quint8> toByteList() const;
    QList<QChar> toCharList() const;
    QList<Decimal> toDecimalList() const;
    QList<double> toDoubleList() const;
    QList<qint16> toInt16List() const;
    QList<qint32> toInt32List() const;
    QList<qint64> toInt64List() const;
    QList<qint8> toSByteList() const;
    QList<float> toSingleList() const;
    QList<TimeSpan> toTimeSpanList() const;
    QList<DateTime> toDateTimeList() const;
    QList<quint16> toUInt16List() const;
    QList<quint32> toUInt32List() const;
    QList<quint64> toUInt64List() const;
    QStringList toStringList() const;

    QStringList asStringList() const;

    bool read(QDataStream &in, int size);
    bool read(QDataStream &in, int size, PrimitiveTypeEnumeration primitiveTypeEnum);

protected:
    QSharedDataPointer<PrimitiveValueArrayData> d;
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_PrimitiveValueArrayARRAY_H
