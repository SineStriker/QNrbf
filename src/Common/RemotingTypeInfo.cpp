#include "RemotingTypeInfo.h"

#include "Primitive/LengthPrefixedString.h"

QNRBF_BEGIN_NAMESPACE

PrimitiveTypeEnumeration RemotingTypeInfo::toPrimitiveTypeEnum() const {
    return _data.value<PrimitiveTypeEnumeration>();
}

QString RemotingTypeInfo::toString() const {
    return _data.toString();
}

ClassTypeInfo RemotingTypeInfo::toClassTypeInfo() const {
    return _data.value<ClassTypeInfo>();
}

bool RemotingTypeInfo::read(QDataStream &in, BinaryTypeEnumeration binaryTypeEnum) {
    switch (binaryTypeEnum) {
        case BinaryTypeEnumeration::Primitive:
        case BinaryTypeEnumeration::PrimitiveArray: {
            PrimitiveTypeEnumeration primitiveTypeEnum;
            if (!Parser::readPrimitiveTypeEnum(primitiveTypeEnum, in)) {
                return false;
            }
            _data.setValue(primitiveTypeEnum);
            break;
        }
        case BinaryTypeEnumeration::SystemClass: {
            QString str;
            if (!Parser::readString(str, in)) {
                return false;
            }
            _data = str;
            break;
        }
        case BinaryTypeEnumeration::Class: {
            ClassTypeInfo info;
            if (!info.read(in)) {
                return false;
            }
            _data.setValue(info);
            break;
        }
        default:
            break;
    }
    return true;
}

QNRBF_END_NAMESPACE
