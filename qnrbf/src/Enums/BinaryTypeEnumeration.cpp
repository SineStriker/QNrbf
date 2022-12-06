#include "BinaryTypeEnumeration.h"

#include <QDataStream>

QNRBF_BEGIN_NAMESPACE

QString Parser::strBinaryTypeEnum(BinaryTypeEnumeration type) {
    QString res;
    switch (type) {
        case BinaryTypeEnumeration::Primitive:
            res = "Primitive";
            break;
        case BinaryTypeEnumeration::String:
            res = "String";
            break;
        case BinaryTypeEnumeration::Object:
            res = "Object";
            break;
        case BinaryTypeEnumeration::SystemClass:
            res = "SystemClass";
            break;
        case BinaryTypeEnumeration::Class:
            res = "Class";
            break;
        case BinaryTypeEnumeration::ObjectArray:
            res = "ObjectArray";
            break;
        case BinaryTypeEnumeration::StringArray:
            res = "StringArray";
            break;
        case BinaryTypeEnumeration::PrimitiveArray:
            res = "PrimitiveArray";
            break;
    }
    return res;
}

bool Parser::readBinaryTypeEnum(BinaryTypeEnumeration &out, QDataStream &in) {
    in >> out;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE