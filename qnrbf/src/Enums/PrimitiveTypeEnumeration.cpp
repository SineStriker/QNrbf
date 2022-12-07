#include "PrimitiveTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

QString Parser::strPrimitiveTypeEnum(PrimitiveTypeEnumeration type) {
    QString res;
    switch (type) {
        case PrimitiveTypeEnumeration::None:
            res = "None";
            break;
        case PrimitiveTypeEnumeration::Boolean:
            res = "Boolean";
            break;
        case PrimitiveTypeEnumeration::Byte:
            res = "Byte";
            break;
        case PrimitiveTypeEnumeration::Char:
            res = "Char";
            break;
        case PrimitiveTypeEnumeration::Unused:
            res = "Unused";
            break;
        case PrimitiveTypeEnumeration::Decimal:
            res = "Decimal";
            break;
        case PrimitiveTypeEnumeration::Double:
            res = "Double";
            break;
        case PrimitiveTypeEnumeration::Int16:
            res = "Int16";
            break;
        case PrimitiveTypeEnumeration::Int32:
            res = "Int32";
            break;
        case PrimitiveTypeEnumeration::Int64:
            res = "Int64";
            break;
        case PrimitiveTypeEnumeration::SByte:
            res = "SByte";
            break;
        case PrimitiveTypeEnumeration::Single:
            res = "Single";
            break;
        case PrimitiveTypeEnumeration::TimeSpan:
            res = "TimeSpan";
            break;
        case PrimitiveTypeEnumeration::DateTime:
            res = "DateTime";
            break;
        case PrimitiveTypeEnumeration::UInt16:
            res = "UInt16";
            break;
        case PrimitiveTypeEnumeration::UInt32:
            res = "UInt32";
            break;
        case PrimitiveTypeEnumeration::UInt64:
            res = "UInt64";
            break;
        case PrimitiveTypeEnumeration::Null:
            res = "Null";
            break;
        case PrimitiveTypeEnumeration::String:
            res = "String";
            break;
        default:
            res = "UnknownPrimitiveType";
            break;
    }
    return res;
}

bool Parser::readPrimitiveTypeEnum(PrimitiveTypeEnumeration &out, QDataStream &in) {
    quint8 primitiveTypeEnum;
    in >> primitiveTypeEnum; // set as 0 if failed
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    if (primitiveTypeEnum == 0 || primitiveTypeEnum > (quint8) PrimitiveTypeEnumeration::String) {
        in.setStatus(QDataStream::ReadCorruptData);
        return false;
    }
    out = (PrimitiveTypeEnumeration) primitiveTypeEnum;
    return true;
}

bool Parser::writePrimitiveTypeEnum(PrimitiveTypeEnumeration in, QDataStream &out){
    out << quint8(in);
    if (out.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE
