#include "BinaryArrayTypeEnumeration.h"

QNRBF_BEGIN_NAMESPACE

QString Parser::strBinaryArrayTypeEnumeration(BinaryArrayTypeEnumeration type) {
    QString res;
    switch (type) {
        case BinaryArrayTypeEnumeration::None:
            res = "None";
            break;
        case BinaryArrayTypeEnumeration::Single:
            res = "Single";
            break;
        case BinaryArrayTypeEnumeration::Jagged:
            res = "Jagged";
            break;
        case BinaryArrayTypeEnumeration::Rectangular:
            res = "Rectangular";
            break;
        case BinaryArrayTypeEnumeration::SingleOffset:
            res = "SingleOffset";
            break;
        case BinaryArrayTypeEnumeration::JaggedOffset:
            res = "JaggedOffset";
            break;
        case BinaryArrayTypeEnumeration::RectangularOffset:
            res = "RectangularOffset";
            break;
        default:
            res = QString("Unknown type %1").arg(QString::number(qint32(type)));
            break;
    }
    return res;
}

bool Parser::readBinaryArrayTypeEnum(BinaryArrayTypeEnumeration &out, QDataStream &in) {
    quint8 binArrayTypeEnum;
    in >> binArrayTypeEnum;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    if (binArrayTypeEnum > (quint8) BinaryArrayTypeEnumeration::RectangularOffset) {
        in.setStatus(QDataStream::ReadCorruptData);
        return false;
    }
    out = (BinaryArrayTypeEnumeration) binArrayTypeEnum;
    return true;
}

bool Parser::writeBinaryArrayTypeEnum(QNrbf::BinaryArrayTypeEnumeration in, QDataStream &out) {
    out << quint8(in);
    if (out.status() != QDataStream::Ok) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE
