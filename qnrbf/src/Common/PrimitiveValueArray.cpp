#include "PrimitiveValueArray.h"
#include "private/PrimitiveValueArrayData.h"

#include <QDebug>

#include "Utils/NrbfParser.h"

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

PrimitiveValueArray::PrimitiveValueArray(PrimitiveTypeEnumeration type, int size)
    : d(new PrimitiveValueArrayData()) {
    d->type = type;
    if (type == PrimitiveTypeEnumeration::None) {
        return;
    }

    d->size = size;
    switch (type) {
        case PrimitiveTypeEnumeration::Boolean:
        case PrimitiveTypeEnumeration::Byte: {
            auto val = new quint8[size];
            memset(val, 0, sizeof(quint8) * size);
            d->data.uc = val;
            break;
        }
        case PrimitiveTypeEnumeration::Char: {
            auto val = new QChar[size];
            d->data.ch = val;
            break;
        }
        case PrimitiveTypeEnumeration::Double: {
            auto val = new double[size];
            memset(val, 0, sizeof(double) * size);
            d->data.d = val;
            break;
        }
        case PrimitiveTypeEnumeration::Int16: {
            auto val = new qint16[size];
            memset(val, 0, sizeof(qint16) * size);
            d->data.s = val;
            break;
        }
        case PrimitiveTypeEnumeration::Int32: {
            auto val = new qint32[size];
            memset(val, 0, sizeof(qint32) * size);
            d->data.i = val;
            break;
        }
        case PrimitiveTypeEnumeration::Int64: {
            auto val = new qint64[size];
            memset(val, 0, sizeof(qint64) * size);
            d->data.l = val;
            break;
        }
        case PrimitiveTypeEnumeration::SByte: {
            auto val = new qint8[size];
            memset(val, 0, sizeof(qint8) * size);
            d->data.c = val;
            break;
        }
        case PrimitiveTypeEnumeration::Single: {
            auto val = new float[size];
            memset(val, 0, sizeof(float) * size);
            d->data.f = val;
            break;
        }
        case PrimitiveTypeEnumeration::TimeSpan: {
            auto val = new TimeSpan[size];
            d->data.ts = val;
            break;
        }
        case PrimitiveTypeEnumeration::DateTime: {
            auto val = new DateTime[size];
            d->data.dt = val;
            break;
        }
        case PrimitiveTypeEnumeration::UInt16: {
            auto val = new quint16[size];
            memset(val, 0, sizeof(quint16) * size);
            d->data.us = val;
            break;
        }
        case PrimitiveTypeEnumeration::UInt32: {
            auto val = new quint32[size];
            memset(val, 0, sizeof(quint32) * size);
            d->data.u = val;
            break;
        }
        case PrimitiveTypeEnumeration::UInt64: {
            auto val = new quint64[size];
            memset(val, 0, sizeof(quint64) * size);
            d->data.ul = val;
            break;
        }
        case PrimitiveTypeEnumeration::Decimal: {
            auto val = new Decimal[size];
            d->data.dec = val;
            break;
        }
        case PrimitiveTypeEnumeration::String: {
            auto val = new QString[size];
            d->data.str = val;
            break;
        }
        default: {
            break;
        }
    }
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

PrimitiveValueArray::PrimitiveValueArray(const QByteArray &bytes, PrimitiveTypeEnumeration type)
    : d(new PrimitiveValueArrayData()) {
    if (type == PrimitiveTypeEnumeration::SByte) {
        d->type = PrimitiveTypeEnumeration::SByte;
        d->size = bytes.size();
        d->data.c = new qint8[bytes.size()];
        std::copy(bytes.begin(), bytes.end(), d->data.c);
    } else {
        d->type = PrimitiveTypeEnumeration::Byte;
        d->size = bytes.size();
        d->data.uc = new quint8[bytes.size()];
        std::copy(bytes.begin(), bytes.end(), d->data.uc);
    }
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
    if (d->type != PrimitiveTypeEnumeration::Boolean)
        return {};
    COPY(bool, b);
}

QList<quint8> PrimitiveValueArray::toByteList() const {
    if (d->type != PrimitiveTypeEnumeration::Byte)
        return {};
    COPY(quint8, uc);
}

QList<QChar> PrimitiveValueArray::toCharList() const {
    if (d->type != PrimitiveTypeEnumeration::Char)
        return {};
    COPY(QChar, ch);
}

QList<Decimal> PrimitiveValueArray::toDecimalList() const {
    if (d->type != PrimitiveTypeEnumeration::Decimal)
        return {};
    COPY(Decimal, dec);
}

QList<double> PrimitiveValueArray::toDoubleList() const {
    if (d->type != PrimitiveTypeEnumeration::Double)
        return {};
    COPY(double, d);
}

QList<qint16> PrimitiveValueArray::toInt16List() const {
    if (d->type != PrimitiveTypeEnumeration::Int16)
        return {};
    COPY(qint16, s);
}

QList<qint32> PrimitiveValueArray::toInt32List() const {
    if (d->type != PrimitiveTypeEnumeration::Int32)
        return {};
    COPY(qint32, i);
}

QList<qint64> PrimitiveValueArray::toInt64List() const {
    if (d->type != PrimitiveTypeEnumeration::Int64)
        return {};
    COPY(qint64, l);
}

QList<qint8> PrimitiveValueArray::toSByteList() const {
    if (d->type != PrimitiveTypeEnumeration::SByte)
        return {};
    COPY(qint8, c);
}

QList<float> PrimitiveValueArray::toSingleList() const {
    if (d->type != PrimitiveTypeEnumeration::Single)
        return {};
    COPY(float, f);
}

QList<TimeSpan> PrimitiveValueArray::toTimeSpanList() const {
    if (d->type != PrimitiveTypeEnumeration::TimeSpan)
        return {};
    COPY(TimeSpan, ts);
}

QList<DateTime> PrimitiveValueArray::toDateTimeList() const {
    if (d->type != PrimitiveTypeEnumeration::DateTime)
        return {};
    COPY(DateTime, dt);
}

QList<quint16> PrimitiveValueArray::toUInt16List() const {
    if (d->type != PrimitiveTypeEnumeration::UInt16)
        return {};
    COPY(quint16, us);
}

QList<quint32> PrimitiveValueArray::toUInt32List() const {
    if (d->type != PrimitiveTypeEnumeration::UInt32)
        return {};
    COPY(quint32, u);
}

QList<quint64> PrimitiveValueArray::toUInt64List() const {
    if (d->type != PrimitiveTypeEnumeration::UInt64)
        return {};
    COPY(quint64, ul);
}

QStringList PrimitiveValueArray::toStringList() const {
    if (d->type != PrimitiveTypeEnumeration::String)
        return {};
    QStringList res;
    res.reserve(d->size);
    for (int i = 0; i < d->size; ++i) {
        res.append(d->data.str[i]);
    }
    return res;
}

QByteArray PrimitiveValueArray::toByteArray() const {
    if (d->type == PrimitiveTypeEnumeration::Byte) {
        return QByteArray(reinterpret_cast<const char *>(d->data.uc), d->size);
    } else if (d->type == PrimitiveTypeEnumeration::SByte) {
        return QByteArray(reinterpret_cast<const char *>(d->data.c), d->size);
    }
    return {};
}

void *PrimitiveValueArray::data() {
    return d->data.ptr;
}

const void *PrimitiveValueArray::constData() const {
    return d->data.ptr;
}

QStringList PrimitiveValueArray::asStringList() const {
    QStringList res;
    res.reserve(d->size);
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
            d->data.c = val;
            break;
        }
        case PrimitiveTypeEnumeration::Single: {
            auto val = new float[size];
            for (int i = 0; i < size; ++i) {
                if (in.readRawData(reinterpret_cast<char *>(&val[i]), sizeof(float)) !=
                    sizeof(float)) {
                    delete[] val;
                    return false;
                }
            }
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
            d->data.str = val;
            break;
        }
        default: {
            // Cannot read other type data
            return false;
            break;
        }
    }
    d->type = primitiveTypeEnum;
    d->size = size;
    return true;
}

bool PrimitiveValueArray::write(QDataStream &out) const {
    // out << qint32(d->size);
    // if (out.status() != QDataStream::Ok) {
    //     return false;
    // }
    switch (d->type) {
        case PrimitiveTypeEnumeration::Boolean:
        case PrimitiveTypeEnumeration::Byte: {
            for (int i = 0; i < d->size; ++i) {
                out << d->data.uc[i];
                if (out.status() != QDataStream::Ok) {
                    return false;
                }
            }
            break;
        }
        case PrimitiveTypeEnumeration::Char: {
            for (int i = 0; i < d->size; ++i) {
                if (!Parser::writeUtf8Char(d->data.ch[i], out)) {
                    return false;
                }
            }
            break;
        }
        case PrimitiveTypeEnumeration::Double: {
            for (int i = 0; i < d->size; ++i) {
                out << d->data.d[i];
                if (out.status() != QDataStream::Ok) {
                    return false;
                }
            }
            break;
        }
        case PrimitiveTypeEnumeration::Int16: {
            for (int i = 0; i < d->size; ++i) {
                out << d->data.s[i];
                if (out.status() != QDataStream::Ok) {
                    return false;
                }
            }
            break;
        }
        case PrimitiveTypeEnumeration::Int32: {
            for (int i = 0; i < d->size; ++i) {
                out << d->data.i[i];
                if (out.status() != QDataStream::Ok) {
                    return false;
                }
            }
            break;
        }
        case PrimitiveTypeEnumeration::Int64: {
            for (int i = 0; i < d->size; ++i) {
                out << d->data.l[i];
                if (out.status() != QDataStream::Ok) {
                    return false;
                }
            }
            break;
        }
        case PrimitiveTypeEnumeration::SByte: {
            for (int i = 0; i < d->size; ++i) {
                out << d->data.c[i];
                if (out.status() != QDataStream::Ok) {
                    return false;
                }
            }
            break;
        }
        case PrimitiveTypeEnumeration::Single: {
            for (int i = 0; i < d->size; ++i) {
                float val = d->data.f[i];
                if (out.writeRawData((char *) &val, sizeof(val)) != sizeof(val)) {
                    return false;
                }
            }
            break;
        }
        case PrimitiveTypeEnumeration::TimeSpan: {
            for (int i = 0; i < d->size; ++i) {
                if (!Parser::writeTimeSpan(d->data.ts[i], out)) {
                    return false;
                }
            }
            break;
        }
        case PrimitiveTypeEnumeration::DateTime: {
            for (int i = 0; i < d->size; ++i) {
                if (!Parser::writeDateTime(d->data.dt[i], out)) {
                    return false;
                }
            }
            break;
        }
        case PrimitiveTypeEnumeration::UInt16: {
            for (int i = 0; i < d->size; ++i) {
                out << d->data.us[i];
                if (out.status() != QDataStream::Ok) {
                    return false;
                }
            }
            break;
        }
        case PrimitiveTypeEnumeration::UInt32: {
            for (int i = 0; i < d->size; ++i) {
                out << d->data.u[i];
                if (out.status() != QDataStream::Ok) {
                    return false;
                }
            }
            break;
        }
        case PrimitiveTypeEnumeration::UInt64: {
            for (int i = 0; i < d->size; ++i) {
                out << d->data.ul[i];
                if (out.status() != QDataStream::Ok) {
                    return false;
                }
            }
            break;
        }
        case PrimitiveTypeEnumeration::Decimal: {
            for (int i = 0; i < d->size; ++i) {
                if (!Parser::writeDecimal(d->data.dec[i], out)) {
                    return false;
                }
            }
            break;
        }
        case PrimitiveTypeEnumeration::String: {
            for (int i = 0; i < d->size; ++i) {
                if (!Parser::writeString(d->data.str[i], out)) {
                    return false;
                }
            }
            break;
        }
        default: {
            // Write nothing and return true
            break;
        }
    }
    return true;
}

bool PrimitiveValueArray::writeWithType(QDataStream &out) const {
    if (!Parser::writePrimitiveTypeEnum(d->type, out)) {
        return false;
    }
    if (!write(out)) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE
