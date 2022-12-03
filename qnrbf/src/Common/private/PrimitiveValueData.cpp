#include "PrimitiveValueData.h"

QNRBF_BEGIN_NAMESPACE

PrimitiveValueData::PrimitiveValueData() : type(PrimitiveTypeEnumeration::None) {
    zeroMem();
}

PrimitiveValueData::PrimitiveValueData(const PrimitiveValueData &other) {
    clone(other);
}

PrimitiveValueData::PrimitiveValueData(PrimitiveValueData &&other) noexcept {
    swap(other);
}

PrimitiveValueData::~PrimitiveValueData() {
    clean();
}

void PrimitiveValueData::zeroMem() {
    memset(&data, 0, sizeof(data));
}

void PrimitiveValueData::clone(const PrimitiveValueData &other) {
    type = other.type;
    switch (type) {
        case PrimitiveTypeEnumeration::Char: {
            data.ch = new QChar(*other.data.ch);
            break;
        }
        case PrimitiveTypeEnumeration::Decimal: {
            data.dec = new Decimal(*other.data.dec);
            break;
        }
        case PrimitiveTypeEnumeration::String: {
            data.str = new QString(*other.data.str);
            break;
        }
        case PrimitiveTypeEnumeration::DateTime: {
            data.dt = new DateTime(*other.data.dt);
            break;
        }
        case PrimitiveTypeEnumeration::TimeSpan: {
            data.ts = new TimeSpan(*other.data.ts);
            break;
        }
        default: {
            memcpy(&data, &other.data, sizeof(data));
            break;
        }
    }
}

void PrimitiveValueData::swap(PrimitiveValueData &other) {
    qSwap(type, other.type);
    qSwap(data, other.data);
}

void PrimitiveValueData::clean() {
    switch (type) {
        case PrimitiveTypeEnumeration::Char: {
            delete data.ch;
            break;
        }
        case PrimitiveTypeEnumeration::Decimal: {
            delete data.dec;
            break;
        }
        case PrimitiveTypeEnumeration::String: {
            delete data.str;
            break;
        }
        case PrimitiveTypeEnumeration::DateTime: {
            delete data.dt;
            break;
        }
        case PrimitiveTypeEnumeration::TimeSpan: {
            delete data.ts;
            break;
        }
        default:
            break;
    }
    zeroMem();
    type = PrimitiveTypeEnumeration::None;
}

QNRBF_END_NAMESPACE
