#ifndef QNRBFFORMAT_PRIMITIVEVALUEDATA_H
#define QNRBFFORMAT_PRIMITIVEVALUEDATA_H

#include "Enums/PrimitiveTypeEnumeration.h"
#include "Primitive/DateTime.h"
#include "Primitive/Decimal.h"
#include "Primitive/TimeSpan.h"

#include <QSharedData>

QNRBF_BEGIN_NAMESPACE

class PrimitiveValueData : public QSharedData {
public:
    union {
        qint8 c;
        quint8 uc;
        qint16 s;
        quint16 us;
        qint32 i;
        quint32 u;
        qint64 l;
        quint64 ul;
        bool b;
        double d;
        float f;
        QChar *ch;
        QString *str;
        Decimal *dec;
        DateTime *dt;
        TimeSpan *ts;
        void *ptr;
    } data;

    PrimitiveTypeEnumeration type;

    PrimitiveValueData();
    PrimitiveValueData(const PrimitiveValueData &other);
    PrimitiveValueData(PrimitiveValueData &&other) noexcept;
    ~PrimitiveValueData();

    void zeroMem();
    void clone(const PrimitiveValueData &other);
    void swap(PrimitiveValueData &other);
    void clean();
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_PRIMITIVEVALUEDATA_H
