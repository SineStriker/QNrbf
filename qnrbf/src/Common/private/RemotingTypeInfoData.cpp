#include "RemotingTypeInfoData.h"

QNRBF_BEGIN_NAMESPACE

RemotingTypeInfoData::RemotingTypeInfoData() : type(RemotingTypeInfo::None) {
    zeroMem();
}

RemotingTypeInfoData::RemotingTypeInfoData(const RemotingTypeInfoData &other) {
    clone(other);
}

RemotingTypeInfoData::RemotingTypeInfoData(RemotingTypeInfoData &&other) noexcept {
    swap(other);
}

RemotingTypeInfoData::~RemotingTypeInfoData() {
    clean();
}

void RemotingTypeInfoData::zeroMem() {
    memset(&data, 0, sizeof(data));
}

void RemotingTypeInfoData::clone(const RemotingTypeInfoData &other) {
    type = other.type;
    switch (type) {
        case RemotingTypeInfo::String: {
            data.str = new QString(*other.data.str);
            break;
        }
        case RemotingTypeInfo::Class: {
            data.cti = new ClassTypeInfo();
            *data.cti = *other.data.cti;
            break;
        }
        default: {
            memcpy(&data, &other.data, sizeof(data));
            break;
        }
    }
}

void RemotingTypeInfoData::swap(RemotingTypeInfoData &other) {
    qSwap(type, other.type);
    qSwap(data, other.data);
}

void RemotingTypeInfoData::clean() {
    switch (type) {
        case RemotingTypeInfo::String: {
            delete data.str;
            break;
        }
        case RemotingTypeInfo::Class: {
            delete data.cti;
            break;
        }
        default:
            break;
    }
    zeroMem();
    type = RemotingTypeInfo::None;
}

QNRBF_END_NAMESPACE
