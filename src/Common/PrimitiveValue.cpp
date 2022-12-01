#include "PrimitiveValue.h"

#include "Primitive/Parser.h"

#include "private/PrimitiveValueData.h"

#include <QVariant>

QNRBF_BEGIN_NAMESPACE

PrimitiveValue::PrimitiveValue() : d(new PrimitiveValueData()) {
}

PrimitiveValue::PrimitiveValue(const PrimitiveValue &other) : d(other.d) {
}

PrimitiveValue &PrimitiveValue::operator=(const PrimitiveValue &other) {
    d = other.d;
    return *this;
}

PrimitiveValue::~PrimitiveValue() {
}

PrimitiveValue::PrimitiveValue(PrimitiveTypeEnumeration nullType) : d(new PrimitiveValueData()) {
    Q_ASSERT(nullType == PrimitiveTypeEnumeration::Null);
    d->type = PrimitiveTypeEnumeration::Null;
}

PrimitiveValue::PrimitiveValue(bool b) : d(new PrimitiveValueData()) {
    d->type = PrimitiveTypeEnumeration::Boolean;
    d->data.b = b;
}

PrimitiveValue::PrimitiveValue(quint8 uc) : d(new PrimitiveValueData()) {
    d->type = PrimitiveTypeEnumeration::Byte;
    d->data.uc = uc;
}

PrimitiveValue::PrimitiveValue(const QChar &ch) : d(new PrimitiveValueData()) {
    d->type = PrimitiveTypeEnumeration::Char;
    d->data.ch = new QChar(ch);
}

PrimitiveValue::PrimitiveValue(const Decimal &dec) : d(new PrimitiveValueData()) {
    d->type = PrimitiveTypeEnumeration::Decimal;
    d->data.dec = new Decimal(dec);
}

PrimitiveValue::PrimitiveValue(double d) : d(new PrimitiveValueData()) {
    this->d->type = PrimitiveTypeEnumeration::Decimal;
    this->d->data.d = d;
}

PrimitiveValue::PrimitiveValue(qint16 s) : d(new PrimitiveValueData()) {
    this->d->type = PrimitiveTypeEnumeration::Int16;
    this->d->data.s = s;
}

PrimitiveValue::PrimitiveValue(qint32 i) : d(new PrimitiveValueData()) {
    d->type = PrimitiveTypeEnumeration::Int32;
    d->data.i = i;
}

PrimitiveValue::PrimitiveValue(qint64 l) : d(new PrimitiveValueData()) {
    d->type = PrimitiveTypeEnumeration::Int64;
    d->data.l = l;
}

PrimitiveValue::PrimitiveValue(qint8 c) : d(new PrimitiveValueData()) {
    d->type = PrimitiveTypeEnumeration::SByte;
    d->data.c = c;
}

PrimitiveValue::PrimitiveValue(float f) : d(new PrimitiveValueData()) {
    d->type = PrimitiveTypeEnumeration::Single;
    d->data.f = f;
}

PrimitiveValue::PrimitiveValue(const TimeSpan &ts) : d(new PrimitiveValueData()) {
    d->type = PrimitiveTypeEnumeration::TimeSpan;
    d->data.ts = new TimeSpan(ts);
}

PrimitiveValue::PrimitiveValue(const DateTime &dt) : d(new PrimitiveValueData()) {
    d->type = PrimitiveTypeEnumeration::DateTime;
    d->data.dt = new DateTime(dt);
}

PrimitiveValue::PrimitiveValue(quint16 us) : d(new PrimitiveValueData()) {
    d->type = PrimitiveTypeEnumeration::UInt16;
    d->data.us = us;
}

PrimitiveValue::PrimitiveValue(quint32 u) : d(new PrimitiveValueData()) {
    d->type = PrimitiveTypeEnumeration::UInt32;
    d->data.u = u;
}

PrimitiveValue::PrimitiveValue(quint64 ul) : d(new PrimitiveValueData()) {
    d->type = PrimitiveTypeEnumeration::UInt64;
    d->data.ul = ul;
}

PrimitiveValue::PrimitiveValue(const QString &str) : d(new PrimitiveValueData()) {
    d->type = PrimitiveTypeEnumeration::String;
    d->data.str = new QString(str);
}

bool PrimitiveValue::isValid() const {
    return d->type != PrimitiveTypeEnumeration::None;
}

PrimitiveTypeEnumeration PrimitiveValue::type() const {
    return d->type;
}

bool PrimitiveValue::toBool() const {
    if (d->type != PrimitiveTypeEnumeration::Boolean) {
        return false;
    }
    return d->data.b;
}

quint8 PrimitiveValue::toByte() const {
    return d->data.uc;
}

QChar PrimitiveValue::toChar() const {
    return *d->data.ch;
}

Decimal PrimitiveValue::toDecimal() const {
    return *d->data.dec;
}

double PrimitiveValue::toDouble() const {
    return d->data.d;
}

qint16 PrimitiveValue::toInt16() const {
    return d->data.s;
}

qint32 PrimitiveValue::toInt32() const {
    return d->data.i;
}

qint64 PrimitiveValue::toInt64() const {
    return d->data.l;
}

qint8 PrimitiveValue::toSByte() const {
    return d->data.c;
}

float PrimitiveValue::toSingle() const {
    return d->data.f;
}

TimeSpan PrimitiveValue::toTimeSpan() const {
    return *d->data.ts;
}

DateTime PrimitiveValue::toDateTime() const {
    return *d->data.dt;
}

quint16 PrimitiveValue::toUInt16() const {
    return d->data.us;
}

quint32 PrimitiveValue::toUInt32() const {
    return d->data.u;
}

quint64 PrimitiveValue::toUInt64() const {
    return d->data.ul;
}

QString PrimitiveValue::toString() const {
    return *d->data.str;
}

bool PrimitiveValue::read(QDataStream &in) {
    PrimitiveTypeEnumeration primitiveTypeEnum;
    if (!Parser::readPrimitiveTypeEnum(primitiveTypeEnum, in)) {
        return false;
    }
    return read(in, primitiveTypeEnum);
}

bool PrimitiveValue::read(QDataStream &in, PrimitiveTypeEnumeration primitiveTypeEnum) {
    switch (primitiveTypeEnum) {
        case PrimitiveTypeEnumeration::Boolean:
        case PrimitiveTypeEnumeration::Byte: {
            quint8 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->type = primitiveTypeEnum;
            d->data.uc = val;
            break;
        }
        case PrimitiveTypeEnumeration::Char: {
            QChar val;
            if (!Parser::readUtf8Char(val, in)) {
                return false;
            }
            d->type = primitiveTypeEnum;
            d->data.ch = new QChar(std::move(val));
            break;
        }
        case PrimitiveTypeEnumeration::Double: {
            double val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->type = primitiveTypeEnum;
            d->data.d = val;
            break;
        }
        case PrimitiveTypeEnumeration::Int16: {
            qint16 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->type = primitiveTypeEnum;
            d->data.s = val;
            break;
        }
        case PrimitiveTypeEnumeration::Int32: {
            qint32 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->type = primitiveTypeEnum;
            d->data.i = val;
            break;
        }
        case PrimitiveTypeEnumeration::Int64: {
            qint64 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->type = primitiveTypeEnum;
            d->data.l = val;
            break;
        }
        case PrimitiveTypeEnumeration::SByte: {
            qint8 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->type = primitiveTypeEnum;
            d->data.c = val;
            break;
        }
        case PrimitiveTypeEnumeration::Single: {
            float val;
            in.readRawData((char *) &val, sizeof(val));
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->type = primitiveTypeEnum;
            d->data.f = val;
            break;
        }
        case PrimitiveTypeEnumeration::TimeSpan: {
            TimeSpan val;
            if (Parser::readTimeSpan(val, in)) {
                return false;
            }
            d->type = primitiveTypeEnum;
            d->data.ts = new TimeSpan(val);
            break;
        }
        case PrimitiveTypeEnumeration::DateTime: {
            DateTime val;
            if (Parser::readDateTime(val, in)) {
                return false;
            }
            d->type = primitiveTypeEnum;
            d->data.dt = new DateTime(val);
            break;
        }
        case PrimitiveTypeEnumeration::UInt16: {
            quint16 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->type = primitiveTypeEnum;
            d->data.us = val;
            break;
        }
        case PrimitiveTypeEnumeration::UInt32: {
            quint32 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->type = primitiveTypeEnum;
            d->data.u = val;
            break;
        }
        case PrimitiveTypeEnumeration::UInt64: {
            quint64 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->type = primitiveTypeEnum;
            d->data.ul = val;
            break;
        }
        case PrimitiveTypeEnumeration::Decimal: {
            Decimal val;
            if (!Parser::readDecimal(val, in)) {
                return false;
            }
            d->type = primitiveTypeEnum;
            d->data.dec = new Decimal(std::move(val));
            break;
        }
        case PrimitiveTypeEnumeration::String: {
            QString val;
            if (!Parser::readString(val, in)) {
                return false;
            }
            d->type = primitiveTypeEnum;
            d->data.str = new QString(std::move(val));
            break;
        }
        default: {
            break;
        }
    }
    return true;
}

QNRBF_END_NAMESPACE
