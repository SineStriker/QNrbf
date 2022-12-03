#include "RemotingTypeInfo.h"

#include "Primitive/Parser.h"

#include "private/RemotingTypeInfoData.h"

QNRBF_BEGIN_NAMESPACE

RemotingTypeInfo::RemotingTypeInfo() : d(new RemotingTypeInfoData()) {
}

RemotingTypeInfo::RemotingTypeInfo(const RemotingTypeInfo &other) : d(other.d) {
}

RemotingTypeInfo &RemotingTypeInfo::operator=(const RemotingTypeInfo &other) {
    d = other.d;
    return *this;
}

RemotingTypeInfo::~RemotingTypeInfo() {
}

RemotingTypeInfo::RemotingTypeInfo(PrimitiveTypeEnumeration pte) : d(new RemotingTypeInfoData()) {
    d->type = PrimitiveType;
    d->data.pte = pte;
}

RemotingTypeInfo::RemotingTypeInfo(const QString &str) {
    d->type = String;
    d->data.str = new QString(str);
}

RemotingTypeInfo::RemotingTypeInfo(const ClassTypeInfo &cti) : d(new RemotingTypeInfoData()) {
    d->type = Class;
    d->data.cti = new ClassTypeInfo();
    *d->data.cti = cti;
}

bool RemotingTypeInfo::isValid() const {
    return d->type != None;
}

RemotingTypeInfo::Type RemotingTypeInfo::type() const {
    return d->type;
}

PrimitiveTypeEnumeration RemotingTypeInfo::toPrimitiveTypeEnum() const {
    return d->data.pte;
}

QString RemotingTypeInfo::toString() const {
    return *d->data.str;
}

ClassTypeInfo RemotingTypeInfo::toClassTypeInfo() const {
    return *d->data.cti;
}

bool RemotingTypeInfo::read(QDataStream &in, BinaryTypeEnumeration binaryTypeEnum) {
    switch (binaryTypeEnum) {
        case BinaryTypeEnumeration::Primitive:
        case BinaryTypeEnumeration::PrimitiveArray: {
            PrimitiveTypeEnumeration primitiveTypeEnum;
            if (!Parser::readPrimitiveTypeEnum(primitiveTypeEnum, in)) {
                return false;
            }
            d->type = PrimitiveType;
            d->data.pte = primitiveTypeEnum;
            break;
        }
        case BinaryTypeEnumeration::SystemClass: {
            QString str;
            if (!Parser::readString(str, in)) {
                return false;
            }
            d->type = String;
            d->data.str = new QString(std::move(str));
            break;
        }
        case BinaryTypeEnumeration::Class: {
            ClassTypeInfo info;
            if (!info.read(in)) {
                return false;
            }
            d->type = Class;
            d->data.cti = new ClassTypeInfo();
            *d->data.cti = info;
            break;
        }
        default:
            break;
    }
    return true;
}

QNRBF_END_NAMESPACE
