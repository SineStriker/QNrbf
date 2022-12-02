#include "PrimitiveValueArray.h"
#include "private/PrimitiveValueArrayData.h"

#include "Primitive/Parser.h"

QNRBF_BEGIN_NAMESPACE

PrimitiveValueArray::PrimitiveValueArray() : d(new PrimitiveValueArrayData()) {
}

PrimitiveValueArray::PrimitiveValueArray(const PrimitiveValueArray &other) : d(other.d) {
}

PrimitiveValueArray &PrimitiveValueArray::operator=(const PrimitiveValueArray &other) {
    d = other.d;
    return *this;
}

PrimitiveValueArray::~PrimitiveValueArray() {
}

PrimitiveValueArray::PrimitiveValueArray(PrimitiveTypeEnumeration nullType, int size)
    : d(new PrimitiveValueArrayData()) {
    Q_ASSERT(nullType == PrimitiveTypeEnumeration::Null);
    d->type = PrimitiveTypeEnumeration::Null;
    d->size = size;
}

#define COPY(ENUM, TYPE, NAME)                                                                     \
    d->type = PrimitiveTypeEnumeration::ENUM;                                                      \
    d->size = NAME##s.size();                                                                      \
    d->data.NAME = new TYPE[NAME##s.size()];                                                       \
    std::copy(NAME##s.begin(), NAME##s.end(), d->data.NAME);

PrimitiveValueArray::PrimitiveValueArray(const QList<bool> &bs) //
    : d(new PrimitiveValueArrayData()) {
    COPY(Boolean, bool, b);
}

PrimitiveValueArray::PrimitiveValueArray(const QList<quint8> &ucs)
    : d(new PrimitiveValueArrayData()) {
    COPY(Byte, quint8, uc);
}

PrimitiveValueArray::PrimitiveValueArray(const QList<QChar> &chs)
    : d(new PrimitiveValueArrayData()) {
    COPY(Char, QChar, ch);
}

PrimitiveValueArray::PrimitiveValueArray(const QList<Decimal> &decs)
    : d(new PrimitiveValueArrayData()) {
    COPY(Decimal, Decimal, dec);
}

PrimitiveValueArray::PrimitiveValueArray(const QList<double> &ds)
    : d(new PrimitiveValueArrayData()) {
    COPY(Double, double, d);
}

PrimitiveValueArray::PrimitiveValueArray(const QList<qint16> &ss)
    : d(new PrimitiveValueArrayData()) {
    COPY(Int16, qint16, s);
}

PrimitiveValueArray::PrimitiveValueArray(const QList<qint32> &is)
    : d(new PrimitiveValueArrayData()) {
    COPY(Int32, qint32, i);
}

PrimitiveValueArray::PrimitiveValueArray(const QList<qint64> &ls)
    : d(new PrimitiveValueArrayData()) {
    COPY(Int64, qint64, l);
}

PrimitiveValueArray::PrimitiveValueArray(const QList<qint8> &cs)
    : d(new PrimitiveValueArrayData()) {
    COPY(SByte, qint8, c);
}

PrimitiveValueArray::PrimitiveValueArray(const QList<float> &fs)
    : d(new PrimitiveValueArrayData()) {
    COPY(Single, float, f);
}

PrimitiveValueArray::PrimitiveValueArray(const QList<TimeSpan> &tss)
    : d(new PrimitiveValueArrayData()) {
    COPY(TimeSpan, TimeSpan, ts);
}

PrimitiveValueArray::PrimitiveValueArray(const QList<DateTime> &dts)
    : d(new PrimitiveValueArrayData()) {
    COPY(DateTime, DateTime, dt);
}

PrimitiveValueArray::PrimitiveValueArray(const QList<quint16> &uss)
    : d(new PrimitiveValueArrayData()) {
    COPY(UInt16, quint16, us);
}

PrimitiveValueArray::PrimitiveValueArray(const QList<quint32> &us)
    : d(new PrimitiveValueArrayData()) {
    COPY(UInt32, quint32, u);
}

PrimitiveValueArray::PrimitiveValueArray(const QList<quint64> &uls)
    : d(new PrimitiveValueArrayData()) {
    COPY(UInt64, quint64, ul);
}

PrimitiveValueArray::PrimitiveValueArray(const QStringList &strs)
    : d(new PrimitiveValueArrayData()) {
    COPY(String, QString, str);
}

#undef COPY

bool PrimitiveValueArray::isValid() const {
    return d->type != PrimitiveTypeEnumeration::None;
}

PrimitiveTypeEnumeration PrimitiveValueArray::type() const {
    return d->type;
}

int PrimitiveValueArray::size() const {
    return d->size;
}

#define COPY(TYPE, NAME)                                                                           \
    QList<TYPE> res;                                                                               \
    res.reserve(d->size);                                                                          \
    for (int i = 0; i < d->size; ++i) {                                                            \
        res.append(d->data.NAME[i]);                                                               \
    }                                                                                              \
    return res;

QList<bool> PrimitiveValueArray::toBoolList() const {
    COPY(bool, b);
}

QList<quint8> PrimitiveValueArray::toByteList() const {
    COPY(quint8, uc);
}

QList<QChar> PrimitiveValueArray::toCharList() const {
    COPY(QChar, ch);
}

QList<Decimal> PrimitiveValueArray::toDecimalList() const {
    COPY(Decimal, dec);
}

QList<double> PrimitiveValueArray::toDoubleList() const {
    COPY(double, d);
}

QList<qint16> PrimitiveValueArray::toInt16List() const {
    COPY(qint16, s);
}

QList<qint32> PrimitiveValueArray::toInt32List() const {
    COPY(qint32, i);
}

QList<qint64> PrimitiveValueArray::toInt64List() const {
    COPY(qint64, l);
}

QList<qint8> PrimitiveValueArray::toSByteList() const {
    COPY(qint8, c);
}

QList<float> PrimitiveValueArray::toSingleList() const {
    COPY(float, f);
}

QList<TimeSpan> PrimitiveValueArray::toTimeSpanList() const {
    COPY(TimeSpan, ts);
}

QList<DateTime> PrimitiveValueArray::toDateTimeList() const {
    COPY(DateTime, dt);
}

QList<quint16> PrimitiveValueArray::toUInt16List() const {
    COPY(quint16, us);
}

QList<quint32> PrimitiveValueArray::toUInt32List() const {
    COPY(quint32, u);
}

QList<quint64> PrimitiveValueArray::toUInt64List() const {
    COPY(quint64, ul);
}

QStringList PrimitiveValueArray::toStringList() const {
    QStringList res;
    res.reserve(d->size);
    for (int i = 0; i < d->size; ++i) {
        res.append(d->data.str[i]);
    }
    return res;
}

QStringList PrimitiveValueArray::asStringList() const {
    QStringList res;
    switch (d->type) {
        case PrimitiveTypeEnumeration::Boolean: {
            auto list = toBoolList();
            for (const auto &item : qAsConst(list)) {
                res.append(item ? "true" : "false");
            }
            break;
        }
        case PrimitiveTypeEnumeration::Byte: {
            auto list = toByteList();
            for (const auto &item : qAsConst(list)) {
                res.append(QString::number(item));
            }
            break;
        }
        case PrimitiveTypeEnumeration::Char: {
            auto list = toCharList();
            for (const auto &item : qAsConst(list)) {
                res.append(item);
            }
            break;
        }
        case PrimitiveTypeEnumeration::Double: {
            auto list = toDoubleList();
            for (const auto &item : qAsConst(list)) {
                res.append(QString::number(item));
            }
            break;
        }
        case PrimitiveTypeEnumeration::Int16: {
            auto list = toInt16List();
            for (const auto &item : qAsConst(list)) {
                res.append(QString::number(item));
            }
            break;
        }
        case PrimitiveTypeEnumeration::Int32: {
            auto list = toInt32List();
            for (const auto &item : qAsConst(list)) {
                res.append(QString::number(item));
            }
            break;
        }
        case PrimitiveTypeEnumeration::Int64: {
            auto list = toInt64List();
            for (const auto &item : qAsConst(list)) {
                res.append(QString::number(item));
            }
            break;
        }
        case PrimitiveTypeEnumeration::SByte: {
            auto list = toSByteList();
            for (const auto &item : qAsConst(list)) {
                res.append(QString::number(item));
            }
            break;
        }
        case PrimitiveTypeEnumeration::Single: {
            auto list = toSingleList();
            for (const auto &item : qAsConst(list)) {
                res.append(QString::number(item));
            }
            break;
        }
        case PrimitiveTypeEnumeration::TimeSpan: {
            auto list = toTimeSpanList();
            for (const auto &item : qAsConst(list)) {
                res.append(QString::number(item._data));
            }
            break;
        }
        case PrimitiveTypeEnumeration::DateTime: {
            auto list = toDateTimeList();
            for (const auto &item : qAsConst(list)) {
                res.append(QString::number(item._data));
            }
            break;
        }
        case PrimitiveTypeEnumeration::UInt16: {
            auto list = toUInt16List();
            for (const auto &item : qAsConst(list)) {
                res.append(QString::number(item));
            }
            break;
        }
        case PrimitiveTypeEnumeration::UInt32: {
            auto list = toUInt32List();
            for (const auto &item : qAsConst(list)) {
                res.append(QString::number(item));
            }
            break;
        }
        case PrimitiveTypeEnumeration::UInt64: {
            auto list = toUInt64List();
            for (const auto &item : qAsConst(list)) {
                res.append(QString::number(item));
            }
            break;
        }
        case PrimitiveTypeEnumeration::Decimal: {
            auto list = toDecimalList();
            for (const auto &item : qAsConst(list)) {
                res.append(item._data);
            }
            break;
        }
        case PrimitiveTypeEnumeration::String: {
            res = toStringList();
            break;
        }
        default: {
            break;
        }
    }
    return res;
}

#undef COPY

bool PrimitiveValueArray::read(QDataStream &in, int size) {
    PrimitiveTypeEnumeration primitiveTypeEnum;
    if (!Parser::readPrimitiveTypeEnum(primitiveTypeEnum, in)) {
        return false;
    }
    return read(in, size, primitiveTypeEnum);
}

bool PrimitiveValueArray::read(QDataStream &in, int size,
                               PrimitiveTypeEnumeration primitiveTypeEnum) {
    switch (primitiveTypeEnum) {
        case PrimitiveTypeEnumeration::Boolean:
        case PrimitiveTypeEnumeration::Byte: {
            auto val = new quint8[size];
            for (int i = 0; i < size; ++i) {
                in >> val[i];
                if (in.status() != QDataStream::Ok) {
                    delete[] val;
                    return false;
                }
            }
            d->type = primitiveTypeEnum;
            d->size = size;
            d->data.uc = val;
            break;
        }
        case PrimitiveTypeEnumeration::Char: {
            auto val = new QChar[size];
            for (int i = 0; i < size; ++i) {
                if (!Parser::readUtf8Char(val[i], in)) {
                    delete[] val;
                    return false;
                }
            }
            d->type = primitiveTypeEnum;
            d->size = size;
            d->data.ch = val;
            break;
        }
        case PrimitiveTypeEnumeration::Double: {
            auto val = new double[size];
            for (int i = 0; i < size; ++i) {
                in >> val[i];
                if (in.status() != QDataStream::Ok) {
                    delete[] val;
                    return false;
                }
            }
            d->type = primitiveTypeEnum;
            d->size = size;
            d->data.d = val;
            break;
        }
        case PrimitiveTypeEnumeration::Int16: {
            auto val = new qint16[size];
            for (int i = 0; i < size; ++i) {
                in >> val[i];
                if (in.status() != QDataStream::Ok) {
                    delete[] val;
                    return false;
                }
            }
            d->type = primitiveTypeEnum;
            d->size = size;
            d->data.s = val;
            break;
        }
        case PrimitiveTypeEnumeration::Int32: {
            auto val = new qint32[size];
            for (int i = 0; i < size; ++i) {
                in >> val[i];
                if (in.status() != QDataStream::Ok) {
                    delete[] val;
                    return false;
                }
            }
            d->type = primitiveTypeEnum;
            d->size = size;
            d->data.i = val;
            break;
        }
        case PrimitiveTypeEnumeration::Int64: {
            auto val = new qint64[size];
            for (int i = 0; i < size; ++i) {
                in >> val[i];
                if (in.status() != QDataStream::Ok) {
                    delete[] val;
                    return false;
                }
            }
            d->type = primitiveTypeEnum;
            d->size = size;
            d->data.l = val;
            break;
        }
        case PrimitiveTypeEnumeration::SByte: {
            auto val = new qint8[size];
            for (int i = 0; i < size; ++i) {
                in >> val[i];
                if (in.status() != QDataStream::Ok) {
                    delete[] val;
                    return false;
                }
            }
            d->type = primitiveTypeEnum;
            d->size = size;
            d->data.c = val;
            break;
        }
        case PrimitiveTypeEnumeration::Single: {
            auto val = new float[size];
            for (int i = 0; i < size; ++i) {
                in >> val[i];
                if (in.status() != QDataStream::Ok) {
                    delete[] val;
                    return false;
                }
            }
            d->type = primitiveTypeEnum;
            d->size = size;
            d->data.f = val;
            break;
        }
        case PrimitiveTypeEnumeration::TimeSpan: {
            auto val = new TimeSpan[size];
            for (int i = 0; i < size; ++i) {
                if (!Parser::readTimeSpan(val[i], in)) {
                    delete[] val;
                    return false;
                }
            }
            d->type = primitiveTypeEnum;
            d->size = size;
            d->data.ts = val;
            break;
        }
        case PrimitiveTypeEnumeration::DateTime: {
            auto val = new DateTime[size];
            for (int i = 0; i < size; ++i) {
                if (!Parser::readDateTime(val[i], in)) {
                    delete[] val;
                    return false;
                }
            }
            d->type = primitiveTypeEnum;
            d->size = size;
            d->data.dt = val;
            break;
        }
        case PrimitiveTypeEnumeration::UInt16: {
            auto val = new quint16[size];
            for (int i = 0; i < size; ++i) {
                in >> val[i];
                if (in.status() != QDataStream::Ok) {
                    delete[] val;
                    return false;
                }
            }
            d->type = primitiveTypeEnum;
            d->size = size;
            d->data.us = val;
            break;
        }
        case PrimitiveTypeEnumeration::UInt32: {
            auto val = new quint32[size];
            for (int i = 0; i < size; ++i) {
                in >> val[i];
                if (in.status() != QDataStream::Ok) {
                    delete[] val;
                    return false;
                }
            }
            d->type = primitiveTypeEnum;
            d->size = size;
            d->data.u = val;
            break;
        }
        case PrimitiveTypeEnumeration::UInt64: {
            auto val = new quint64[size];
            for (int i = 0; i < size; ++i) {
                in >> val[i];
                if (in.status() != QDataStream::Ok) {
                    delete[] val;
                    return false;
                }
            }
            d->type = primitiveTypeEnum;
            d->size = size;
            d->data.ul = val;
            break;
        }
        case PrimitiveTypeEnumeration::Decimal: {
            auto val = new Decimal[size];
            for (int i = 0; i < size; ++i) {
                if (!Parser::readDecimal(val[i], in)) {
                    delete[] val;
                    return false;
                }
            }
            d->type = primitiveTypeEnum;
            d->size = size;
            d->data.dec = val;
            break;
        }
        case PrimitiveTypeEnumeration::String: {
            auto val = new QString[size];
            for (int i = 0; i < size; ++i) {
                if (!Parser::readString(val[i], in)) {
                    delete[] val;
                    return false;
                }
            }
            d->type = primitiveTypeEnum;
            d->size = size;
            d->data.str = val;
            break;
        }
        default: {
            break;
        }
    }
    return true;
}

QNRBF_END_NAMESPACE
