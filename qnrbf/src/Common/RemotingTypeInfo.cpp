#include "RemotingTypeInfo.h"

#include "Utils/NrbfParser.h"

#include "private/RemotingTypeInfoData.h"

#include <QJsonObject>

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

RemotingTypeInfo::RemotingTypeInfo(const QString &str) : d(new RemotingTypeInfoData()) {
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
    return d->type == PrimitiveType ? d->data.pte : PrimitiveTypeEnumeration::None;
}

QString RemotingTypeInfo::toString() const {
    return d->type == String ? *d->data.str : QString();
}

ClassTypeInfo RemotingTypeInfo::toClassTypeInfo() const {
    return d->type == Class ? *d->data.cti : ClassTypeInfo();
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
            *d->data.cti = std::move(info);
            break;
        }
        default:
            // Treat other type as nothing
            d->type = None;
            break;
    }
    return true;
}

bool RemotingTypeInfo::write(QDataStream &out) const {
    switch (d->type) {
        case PrimitiveType: {
            if (!Parser::writePrimitiveTypeEnum(d->data.pte, out)) {
                return false;
            }
            break;
        }
        case String: {
            if (!Parser::writeString(*d->data.str, out)) {
                return false;
            }
            break;
        }
        case Class: {
            if (!d->data.cti->write(out)) {
                return false;
            }
            break;
        }
        default:
            // Write nothing and return true
            break;
    }
    return true;
}

QJsonValue RemotingTypeInfo::readableTypeInfo() const {
    QJsonValue infoValue(QJsonValue::Null);
    switch (d->type) {
        case QNrbf::RemotingTypeInfo::PrimitiveType:
            infoValue = QNrbf::Parser::strPrimitiveTypeEnum(d->data.pte);
            break;
        case QNrbf::RemotingTypeInfo::String:
            infoValue = *d->data.str;
            break;
        case QNrbf::RemotingTypeInfo::Class: {
            infoValue = QJsonObject(
                {{"typeName", d->data.cti->typeName}, {"libraryId", d->data.cti->libraryId}});
            break;
        }
        default:
            break;
    }
    return infoValue;
}

QNRBF_END_NAMESPACE
