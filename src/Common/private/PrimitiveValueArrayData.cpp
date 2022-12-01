#include "PrimitiveValueArrayData.h"

QNRBF_BEGIN_NAMESPACE

PrimitiveValueArrayData::PrimitiveValueArrayData() : type(PrimitiveTypeEnumeration::None) {
    zeroMem();
}

PrimitiveValueArrayData::PrimitiveValueArrayData(const PrimitiveValueArrayData &other) {
    clone(other);
}

PrimitiveValueArrayData::PrimitiveValueArrayData(PrimitiveValueArrayData &&other) noexcept {
    swap(other);
}

PrimitiveValueArrayData::~PrimitiveValueArrayData() {
    clean();
}

void PrimitiveValueArrayData::zeroMem() {
    memset(&data, 0, sizeof(data));
}

void PrimitiveValueArrayData::clone(const PrimitiveValueArrayData &other) {
    type = other.type;
    size = other.size;

#define COPY(ENUM, TYPE, NAME)                                                                     \
    case PrimitiveTypeEnumeration::ENUM: {                                                         \
        data.NAME = new TYPE[size];                                                                \
        std::copy(other.data.NAME, other.data.NAME + other.size, data.NAME);                       \
        break;                                                                                     \
    }

    switch (type) {
        COPY(Boolean, bool, b);
        COPY(Byte, quint8, uc);
        COPY(Char, QChar, ch);
        COPY(Decimal, Decimal, dec);
        COPY(Double, double, d);
        COPY(Int16, qint16, s);
        COPY(Int32, qint32, i);
        COPY(Int64, qint64, l);
        COPY(SByte, qint8, c);
        COPY(Single, float, f);
        COPY(TimeSpan, TimeSpan, ts);
        COPY(DateTime, DateTime, dt);
        COPY(UInt16, quint16, us);
        COPY(UInt32, quint32, u);
        COPY(UInt64, quint64, ul);
        COPY(String, QString, str);
        default: {
            // skip Null
            break;
        }
    }
#undef COPY
}

void PrimitiveValueArrayData::swap(PrimitiveValueArrayData &other) {
    qSwap(type, other.type);
    qSwap(size, other.size);
    qSwap(data, other.data);
}

void PrimitiveValueArrayData::clean() {
#define DEL(ENUM, NAME)                                                                            \
    case PrimitiveTypeEnumeration::ENUM: {                                                         \
        delete[] data.NAME;                                                                        \
        break;                                                                                     \
    }

    switch (type) {
        DEL(Boolean, b);
        DEL(Byte, uc);
        DEL(Char, ch);
        DEL(Decimal, dec);
        DEL(Double, d);
        DEL(Int16, s);
        DEL(Int32, i);
        DEL(Int64, l);
        DEL(SByte, c);
        DEL(Single, f);
        DEL(TimeSpan, ts);
        DEL(DateTime, dt);
        DEL(UInt16, us);
        DEL(UInt32, u);
        DEL(UInt64, ul);
        DEL(String, str);
        default: {
            // skip Null
            break;
        }
    }
    zeroMem();
    type = PrimitiveTypeEnumeration::None;
#undef COPY
}

QNRBF_END_NAMESPACE
