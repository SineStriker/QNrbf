#include "LengthPrefixedString.h"

QNRBF_BEGIN_NAMESPACE

bool Parser::readLengthPrefix(quint32 &size, QDataStream &in) {
    size = 0;

    quint8 flag = 0b10000000;
    quint8 mask = 0b01111111;
    quint8 byte;
    quint8 offset = 0;

    // 1st byte
    in >> byte;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    size |= byte & mask;
    if (!(byte & flag)) {
        return true;
    }

    // 2nd/3rd/4th byte
    for (int8_t i = 0; i < 3; ++i) {
        in >> byte;
        if (in.status() != QDataStream::Ok) {
            return false;
        }
        offset += 7;
        size |= quint32(byte & mask) << offset;
        if (!(byte & flag)) {
            return true;
        }
    }

    quint8 mask2 = 0b00000111;

    // 5th byte
    in >> byte;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    offset += 3;
    size |= quint32(byte & mask2) << offset;
    // High 5 bits must be 0
    if (byte & (~mask2)) {
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

    // Read bytes
    auto buf = new char[size];
    if (in.readRawData(buf, size) < 0) {
        delete[] buf;
        return false;
    }

    // Decode
    out = QString::fromUtf8(buf, size);
    delete[] buf;

    return true;
}

QNRBF_END_NAMESPACE