#ifndef QNRBFFORMAT_PRIMITIVEVALUEARRAYDATA_H
#define QNRBFFORMAT_PRIMITIVEVALUEARRAYDATA_H

#include "Enums/PrimitiveTypeEnumeration.h"
#include "Primitive/DateTime.h"
#include "Primitive/Decimal.h"
#include "Primitive/TimeSpan.h"

#include <QSharedData>

QNRBF_BEGIN_NAMESPACE

class PrimitiveValueArrayData : public QSharedData {
public:
    union {
        qint8 *c;
        quint8 *uc;
        qint16 *s;
        quint16 *us;
        qint32 *i;
        quint32 *u;
        qint64 *l;
        quint64 *ul;
        bool *b;
        double *d;
        float *f;
        QChar *ch;
        QString *str;
        Decimal *dec;
        DateTime *dt;
        TimeSpan *ts;
        void *ptr;
    } data;

    int size;
    PrimitiveTypeEnumeration type;

    PrimitiveValueArrayData();
    PrimitiveValueArrayData(const PrimitiveValueArrayData &other);
    PrimitiveValueArrayData(PrimitiveValueArrayData &&other) noexcept;
    ~PrimitiveValueArrayData();

    void zeroMem();
    void clone(const PrimitiveValueArrayData &other);
    void swap(PrimitiveValueArrayData &other);
    void clean();
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_PRIMITIVEVALUEARRAYDATA_H
