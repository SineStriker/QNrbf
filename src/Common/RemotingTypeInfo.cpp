#include "RemotingTypeInfo.h"

QNRBF_BEGIN_NAMESPACE

QNrbf::PrimitiveTypeEnumeration QNrbf::RemotingTypeInfo::toPrimitiveTypeEnum() const {
    return QNrbf::PrimitiveTypeEnumeration::Int16;
}

QString QNrbf::RemotingTypeInfo::toString() const {
    return QString();
}

QNrbf::ClassTypeInfo QNrbf::RemotingTypeInfo::toClassTypeInfo() const {
    return QNrbf::ClassTypeInfo();
}


QNRBF_END_NAMESPACE
