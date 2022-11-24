#include "PrimitiveValue.h"

#include "Primitive/LengthPrefixedString.h"

QNRBF_BEGIN_NAMESPACE

bool PrimitiveValue::toBool() const {
    return _data.toBool();
}

quint8 PrimitiveValue::toByte() const {
    return _data.toInt();
}

QChar PrimitiveValue::toChar() const {
    return _data.toChar();
}

QString PrimitiveValue::toDecimal() const {
    return _data.toString();
}

double PrimitiveValue::toDouble() const {
    return _data.toDouble();
}

qint16 PrimitiveValue::toInt16() const {
    return (qint16) _data.toInt();
}

qint32 PrimitiveValue::toInt32() const {
    return _data.toInt();
}

qint64 PrimitiveValue::toInt64() const {
    return _data.toLongLong();
}

qint8 PrimitiveValue::toSByte() const {
    return (qint8) _data.toInt();
}

float PrimitiveValue::toSingle() const {
    return _data.toFloat();
}

TimeSpan PrimitiveValue::toTimeSpan() const {
    return _data.value<TimeSpan>();
}

DateTime PrimitiveValue::toDateTime() const {
    return _data.value<DateTime>();
}

quint16 PrimitiveValue::toUInt16() const {
    return _data.toInt();
}

quint32 PrimitiveValue::toUInt32() const {
    return _data.toUInt();
}

quint64 PrimitiveValue::toUInt64() const {
    return _data.toULongLong();
}

QString PrimitiveValue::toString() const {
    return _data.toString();
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
            _data = (int) val;
            break;
        }
        case PrimitiveTypeEnumeration::Char: {
            quint8 head;
            in >> head;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            int size = (head & 0xD0) != 0 ? 4 : (head & 0xC0) != 0 ? 3 : (head & 0x80) != 0 ? 2 : 1;
            char buf[4];
            if (in.readRawData(buf, size) < 0) {
                return false;
            }
            ushort value = (head & 0xD0) != 0 ? ((head & 0x3F) << 18) + ((buf[0] & 0x3F) << 12) +
                                                    ((buf[1] & 0x3F) << 6) + (buf[0] & 0x3F)
                           : (head & 0xC0) != 0
                               ? ((head & 0x3F) << 12) + ((buf[0] & 0x3F) << 6) + (buf[1] & 0x3F)
                           : (head & 0x80) != 0 ? ((head & 0x3F) << 6) + (buf[0] & 0x3F)
                                                : (head & 0x3F);
            _data = QChar(value);
            break;
        }
        case PrimitiveTypeEnumeration::Double: {
            double val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            _data = val;
            break;
        }
        case PrimitiveTypeEnumeration::Int16: {
            qint16 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            _data =(int) val;
            break;
        }
        case PrimitiveTypeEnumeration::Int32: {
            qint32 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            _data = val;
            break;
        }
        case PrimitiveTypeEnumeration::TimeSpan:
        case PrimitiveTypeEnumeration::Int64: {
            qint64 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            _data = val;
            break;
        }
        case PrimitiveTypeEnumeration::SByte: {
            qint8 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            _data = (int) val;
            break;
        }
        case PrimitiveTypeEnumeration::Single: {
            float val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            _data = val;
            break;
        }
        case PrimitiveTypeEnumeration::DateTime: {
            qint64 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            DateTime dt;
            dt._data = val;
            _data.setValue(dt);
            break;
        }
        case PrimitiveTypeEnumeration::UInt16: {
            quint16 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            _data = (int) val;
            break;
        }
        case PrimitiveTypeEnumeration::UInt32: {
            quint32 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            _data = val;
            break;
        }
        case PrimitiveTypeEnumeration::UInt64: {
            quint64 val;
            in >> val;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            _data = val;
            break;
        }
        case PrimitiveTypeEnumeration::Decimal:
        case PrimitiveTypeEnumeration::String: {
            QString str;
            if (!Parser::readString(str, in)) {
                return false;
            }
            _data = str;
            break;
        }
        default: {
            break;
        }
    }
    return true;
}

QNRBF_END_NAMESPACE
