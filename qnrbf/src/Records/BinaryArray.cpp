#include "BinaryArray.h"

QNRBF_BEGIN_NAMESPACE

BinaryArray::BinaryArray() {
    objectId = 0;
    binaryTypeEnum = BinaryTypeEnumeration::None;
    binaryArrayTypeEnum = BinaryArrayTypeEnumeration::None;
    rank = 0;
}

bool BinaryArray::read(QDataStream &in) {
    in >> objectId;
    if (in.status() != QDataStream::Ok) {
        return false;
    }

    if (!Parser::readBinaryArrayTypeEnum(binaryArrayTypeEnum, in)) {
        return false;
    }

    in >> rank;
    if (in.status() != QDataStream::Ok) {
        return false;
    }

    for (qint32 i = 0; i < rank; ++i) {
        qint32 num;
        in >> num;
        if (in.status() != QDataStream::Ok || num < 0) {
            return false;
        }
        lengths.append(num);
    }

    switch (binaryArrayTypeEnum) {
        case QNrbf::BinaryArrayTypeEnumeration::SingleOffset:
        case QNrbf::BinaryArrayTypeEnumeration::JaggedOffset:
        case QNrbf::BinaryArrayTypeEnumeration::RectangularOffset: {
            for (qint32 i = 0; i < rank; ++i) {
                qint32 num;
                in >> num;
                if (in.status() != QDataStream::Ok) {
                    return false;
                }
                lowerBounds.append(num);
            }
            break;
        }
        default:
            break;
    }

    if (!Parser::readBinaryTypeEnum(binaryTypeEnum, in)) {
        return false;
    }
    if (!additionInfo.read(in, binaryTypeEnum)) {
        return false;
    }
    return true;
}

bool BinaryArray::write(QDataStream &out) const {
    out << objectId;
    if (out.status() != QDataStream::Ok) {
        return false;
    }

    if (!Parser::writeBinaryArrayTypeEnum(binaryArrayTypeEnum, out)) {
        return false;
    }

    out << rank;
    if (out.status() != QDataStream::Ok) {
        return false;
    }

    for (qint32 i = 0; i < rank; ++i) {
        out << lengths.at(i);
        if (out.status() != QDataStream::Ok) {
            return false;
        }
    }

    switch (binaryArrayTypeEnum) {
        case QNrbf::BinaryArrayTypeEnumeration::SingleOffset:
        case QNrbf::BinaryArrayTypeEnumeration::JaggedOffset:
        case QNrbf::BinaryArrayTypeEnumeration::RectangularOffset: {
            for (qint32 i = 0; i < rank; ++i) {
                out << lowerBounds.at(i);
                if (out.status() != QDataStream::Ok) {
                    return false;
                }
            }
            break;
        }
        default:
            break;
    }

    if (!Parser::writeBinaryTypeEnum(binaryTypeEnum, out)) {
        return false;
    }
    if (!additionInfo.write(out)) {
        return false;
    }
    return true;
}

QNRBF_END_NAMESPACE
