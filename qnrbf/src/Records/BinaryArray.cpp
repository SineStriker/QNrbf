#include "BinaryArray.h"

QNRBF_BEGIN_NAMESPACE

BinaryArray::BinaryArray() {
    objectId = 0;
    binaryArrayTypeEnum = BinaryArrayTypeEnumeration::Single;
    rank = 0;
}

bool BinaryArray::read(QDataStream &in) {
    in >> objectId;

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

    QNrbf::BinaryTypeEnumeration binaryTypeEnum;
    if (!Parser::readBinaryTypeEnum(binaryTypeEnum, in)) {
        return false;
    }
    if (!additionInfo.read(in, binaryTypeEnum)) {
        return false;
    }
    return true;
}

bool QNrbf::BinaryArray::write(QDataStream &out) const {
    return false;
}

QNRBF_END_NAMESPACE
