#include "BinaryTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

QString Parser::strBinaryTypeEnum(BinaryTypeEnumeration type) {
    QString res;
    switch (type) {
        case BinaryTypeEnumeration::None:
            res = "None";
            break;
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
        default:
            res = QString("Unknown type %1").arg(QString::number(qint32(type)));
            break;
    }
    return res;
}

bool Parser::readBinaryTypeEnum(BinaryTypeEnumeration &out, QDataStream &in) {
    quint8 binaryTypeEnum;
    in >> binaryTypeEnum;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    if (binaryTypeEnum > (quint8) BinaryTypeEnumeration::PrimitiveArray) {
        in.setStatus(QDataStream::ReadCorruptData);
        return false;
    }
    out = (BinaryTypeEnumeration) binaryTypeEnum;
    return true;
}

bool Parser::writeBinaryTypeEnum(BinaryTypeEnumeration in, QDataStream &out) {
    out << quint8(in);
    if (out.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE