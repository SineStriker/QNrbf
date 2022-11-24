#include "LengthPrefixedString.h"

#include <QDebug>

QNRBF_BEGIN_NAMESPACE

static const quint8 mask_high1 = 0b10000000;
static const quint8 mask_low7 = 0b01111111;
static const quint8 mask_low3 = 0b00000111;

bool Parser::readLengthPrefix(quint32 &size, QDataStream &in) {
    size = 0;

    quint8 byte;
    quint8 offset = 0;

    // 1st byte
    in >> byte;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    size |= byte & mask_low7;
    if (!(byte & mask_high1)) {
        return true;
    }

    // 2nd/3rd/4th byte
    for (int8_t i = 0; i < 3; ++i) {
        in >> byte;
        if (in.status() != QDataStream::Ok) {
            return false;
        }
        offset += 7;
        size |= quint32(byte & mask_low7) << offset;
        if (!(byte & mask_high1)) {
            return true;
        }
    }

    // 5th byte
    in >> byte;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    offset += 3;
    size |= quint32(byte & mask_low3) << offset;
    // High 5 bits must be 0
    if (byte & (~mask_low3)) {
        return false;
    }

    return true;
}

bool Parser::readString(QString &out, QDataStream &in) {
    quint32 size;
    // Parse size
    if (!readLengthPrefix(size, in)) {
        return false;
    }

    // Check size valid
    auto dev = in.device();
    if (dev->size() - dev->pos() < (qint64) size) {
        in.setStatus(QDataStream::ReadPastEnd);
        return false;
    }

    // read bytes
    auto buf = new char[size];
    if (in.readRawData(buf, (int) size) < 0) {
        delete[] buf;
        return false;
    }

    // Decode
    out = QString::fromUtf8(buf, (int) size);
    delete[] buf;

    qDebug() << out;

    return true;
}

QNRBF_END_NAMESPACE