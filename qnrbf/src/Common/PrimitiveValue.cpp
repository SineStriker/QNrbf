#include "PrimitiveValue.h"

#include "Utils/NrbfParser.h"

#include "private/PrimitiveValueData.h"

#include <QDebug>

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

PrimitiveValue::PrimitiveValue(PrimitiveTypeEnumeration type) : d(new PrimitiveValueData()) {
    d->type = type;
    switch (type) {
        case PrimitiveTypeEnumeration::Char:
            d->data.ch = new QChar();
            break;
        case PrimitiveTypeEnumeration::String:
            d->data.str = new QString();
            break;
        case PrimitiveTypeEnumeration::Decimal:
            d->data.dec = new Decimal();
            break;
        case PrimitiveTypeEnumeration::DateTime:
            d->data.dt = new DateTime();
            break;
        case PrimitiveTypeEnumeration::TimeSpan:
            d->data.ts = new TimeSpan();
            break;
        default:
            break;
    }
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
    this->d->type = PrimitiveTypeEnumeration::Double;
    this->d->data.d = d;
}

PrimitiveValue::PrimitiveValue(qint16 s) : d(new PrimitiveValueData()) {
    d->type = PrimitiveTypeEnumeration::Int16;
    d->data.s = s;
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
    return d->type == PrimitiveTypeEnumeration::Boolean && d->data.b;
}

quint8 PrimitiveValue::toByte() const {
    return d->type == PrimitiveTypeEnumeration::Byte ? d->data.uc : quint8(0);
}

QChar PrimitiveValue::toChar() const {
    return d->type == PrimitiveTypeEnumeration::Char ? *d->data.ch : QChar();
}

Decimal PrimitiveValue::toDecimal() const {
    return d->type == PrimitiveTypeEnumeration::Decimal ? *d->data.dec : Decimal();
}

double PrimitiveValue::toDouble() const {
    return d->type == PrimitiveTypeEnumeration::Double ? d->data.d : 0.0;
}

qint16 PrimitiveValue::toInt16() const {
    return d->type == PrimitiveTypeEnumeration::Int16 ? d->data.s : qint16(0);
}

qint32 PrimitiveValue::toInt32() const {
    return d->type == PrimitiveTypeEnumeration::Int32 ? d->data.i : qint32(0);
}

qint64 PrimitiveValue::toInt64() const {
    return d->type == PrimitiveTypeEnumeration::Int64 ? d->data.l : qint64(0);
}

qint8 PrimitiveValue::toSByte() const {
    return d->type == PrimitiveTypeEnumeration::SByte ? d->data.c : qint8(0);
}

float PrimitiveValue::toSingle() const {
    return d->type == PrimitiveTypeEnumeration::Single ? d->data.f : float(0);
}

TimeSpan PrimitiveValue::toTimeSpan() const {
    return d->type == PrimitiveTypeEnumeration::TimeSpan ? *d->data.ts : TimeSpan();
}

DateTime PrimitiveValue::toDateTime() const {
    return d->type == PrimitiveTypeEnumeration::DateTime ? *d->data.dt : DateTime();
}

quint16 PrimitiveValue::toUInt16() const {
    return d->type == PrimitiveTypeEnumeration::UInt16 ? d->data.us : quint16(0);
}

quint32 PrimitiveValue::toUInt32() const {
    return d->type == PrimitiveTypeEnumeration::UInt32 ? d->data.u : quint32(0);
}

quint64 PrimitiveValue::toUInt64() const {
    return d->type == PrimitiveTypeEnumeration::Int64 ? d->data.ul : quint64(0);
}

QString PrimitiveValue::toString() const {
    return d->type == PrimitiveTypeEnumeration::String ? *d->data.str : QString();
}

void *PrimitiveValue::data() {
    void *res;
    switch (d->type) {
        case PrimitiveTypeEnumeration::Char:
        case PrimitiveTypeEnumeration::Decimal:
        case PrimitiveTypeEnumeration::String:
        case PrimitiveTypeEnumeration::DateTime:
        case PrimitiveTypeEnumeration::TimeSpan: {
            res = d->data.ptr;
            break;
        }
        default:
            res = &d->data;
            break;
    }
    return res;
}

const void *PrimitiveValue::constData() const {
    const void *res;
    switch (d->type) {
        case PrimitiveTypeEnumeration::Char:
        case PrimitiveTypeEnumeration::Decimal:
        case PrimitiveTypeEnumeration::String:
        case PrimitiveTypeEnumeration::DateTime:
        case PrimitiveTypeEnumeration::TimeSpan: {
            res = d->data.ptr;
            break;
        }
        default:
            res = &d->data;
            break;
    }
    return res;
}

QString PrimitiveValue::asString() const {
    QString res;
    switch (d->type) {
        case PrimitiveTypeEnumeration::Boolean: {
            res = toBool() ? "true" : "false";
            break;
        }
        case PrimitiveTypeEnumeration::Byte: {
            res = QString::number(toByte());
            break;
        }
        case PrimitiveTypeEnumeration::Char: {
            res = toChar();
            break;
        }
        case PrimitiveTypeEnumeration::Double: {
            res = QString::number(toDouble());
            break;
        }
        case PrimitiveTypeEnumeration::Int16: {
            res = QString::number(toInt16());
            break;
        }
        case PrimitiveTypeEnumeration::Int32: {
            res = QString::number(toInt32());
            break;
        }
        case PrimitiveTypeEnumeration::Int64: {
            res = QString::number(toInt64());
            break;
        }
        case PrimitiveTypeEnumeration::SByte: {
            res = QString::number(toSByte());
            break;
        }
        case PrimitiveTypeEnumeration::Single: {
            res = QString::number(toSingle());
            break;
        }
        case PrimitiveTypeEnumeration::TimeSpan: {
            res = QString::number(toTimeSpan().ticks);
            break;
        }
        case PrimitiveTypeEnumeration::DateTime: {
            res = QString::number(toDateTime()._data);
            break;
        }
        case PrimitiveTypeEnumeration::UInt16: {
            res = QString::number(toUInt16());
            break;
        }
        case PrimitiveTypeEnumeration::UInt32: {
            res = QString::number(toUInt32());
            break;
        }
        case PrimitiveTypeEnumeration::UInt64: {
            res = QString::number(toUInt64());
            break;
        }
        case PrimitiveTypeEnumeration::Decimal: {
            res = toDecimal()._data;
            break;
        }
        case PrimitiveTypeEnumeration::String: {
            res = toString();
            break;
        }
        default: {
            break;
        }
    }
    return res;
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
            d->data.uc = val;
            break;
        }
        case PrimitiveTypeEnumeration::Char: {
            QChar val;
            if (!Parser::readUtf8Char(val, in)) {
                return false;
            }
            d->data.ch = new QChar(std::move(val));
            break;
        }
        case PrimitiveTypeEnumeration::Double: {
            double val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->data.d = val;
            break;
        }
        case PrimitiveTypeEnumeration::Int16: {
            qint16 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->data.s = val;
            break;
        }
        case PrimitiveTypeEnumeration::Int32: {
            qint32 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->data.i = val;
            break;
        }
        case PrimitiveTypeEnumeration::Int64: {
            qint64 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->data.l = val;
            break;
        }
        case PrimitiveTypeEnumeration::SByte: {
            qint8 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->data.c = val;
            break;
        }
        case PrimitiveTypeEnumeration::Single: {
            float val;
            in.readRawData((char *) &val, sizeof(val));
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->data.f = val;
            break;
        }
        case PrimitiveTypeEnumeration::TimeSpan: {
            TimeSpan val;
            if (!Parser::readTimeSpan(val, in)) {
                return false;
            }
            d->data.ts = new TimeSpan(val);
            break;
        }
        case PrimitiveTypeEnumeration::DateTime: {
            DateTime val;
            if (!Parser::readDateTime(val, in)) {
                return false;
            }
            d->data.dt = new DateTime(val);
            break;
        }
        case PrimitiveTypeEnumeration::UInt16: {
            quint16 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->data.us = val;
            break;
        }
        case PrimitiveTypeEnumeration::UInt32: {
            quint32 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->data.u = val;
            break;
        }
        case PrimitiveTypeEnumeration::UInt64: {
            quint64 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            d->data.ul = val;
            break;
        }
        case PrimitiveTypeEnumeration::Decimal: {
            Decimal val;
            if (!Parser::readDecimal(val, in)) {
                return false;
            }
            d->data.dec = new Decimal(std::move(val));
            break;
        }
        case PrimitiveTypeEnumeration::String: {
            QString val;
            if (!Parser::readString(val, in)) {
                return false;
            }
            d->data.str = new QString(std::move(val));
            break;
        }
        default: {
            // Cannot read other type data
            return false;
            break;
        }
    }
    d->type = primitiveTypeEnum;

    return true;
}

bool PrimitiveValue::write(QDataStream &out) const {
    bool success = true;
    switch (d->type) {
        case PrimitiveTypeEnumeration::Boolean:
        case PrimitiveTypeEnumeration::Byte: {
            out << d->data.uc;
            success = out.status() == QDataStream::Ok;
            break;
        }
        case PrimitiveTypeEnumeration::Char: {
            success = Parser::writeUtf8Char(*d->data.ch, out);
            break;
        }
        case PrimitiveTypeEnumeration::Double: {
            out << d->data.d;
            success = out.status() == QDataStream::Ok;
            break;
        }
        case PrimitiveTypeEnumeration::Int16: {
            out << d->data.s;
            success = out.status() == QDataStream::Ok;
            break;
        }
        case PrimitiveTypeEnumeration::Int32: {
            out << d->data.i;
            success = out.status() == QDataStream::Ok;
            break;
        }
        case PrimitiveTypeEnumeration::Int64: {
            out << d->data.l;
            success = out.status() == QDataStream::Ok;
            break;
        }
        case PrimitiveTypeEnumeration::SByte: {
            out << d->data.c;
            success = out.status() == QDataStream::Ok;
            break;
        }
        case PrimitiveTypeEnumeration::Single: {
            float val = d->data.f;
            success = out.writeRawData((char *) &val, sizeof(val)) == sizeof(val);
            break;
        }
        case PrimitiveTypeEnumeration::TimeSpan: {
            success = Parser::writeTimeSpan(*d->data.ts, out);
            break;
        }
        case PrimitiveTypeEnumeration::DateTime: {
            success = Parser::writeDateTime(*d->data.dt, out);
            break;
        }
        case PrimitiveTypeEnumeration::UInt16: {
            out << d->data.us;
            success = out.status() == QDataStream::Ok;
            break;
        }
        case PrimitiveTypeEnumeration::UInt32: {
            out << d->data.u;
            success = out.status() == QDataStream::Ok;
            break;
        }
        case PrimitiveTypeEnumeration::UInt64: {
            out << d->data.ul;
            success = out.status() == QDataStream::Ok;
            break;
        }
        case PrimitiveTypeEnumeration::Decimal: {
            success = Parser::writeDecimal(*d->data.dec, out);
            break;
        }
        case PrimitiveTypeEnumeration::String: {
            success = Parser::writeString(*d->data.str, out);
            break;
        }
        default: {
            // Write nothing and return true
            break;
        }
    }
    return success;
}

bool PrimitiveValue::writeWithType(QDataStream &out) const {
    if (!Parser::writePrimitiveTypeEnum(d->type, out)) {
        return false;
    }
    if (!write(out)) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE
