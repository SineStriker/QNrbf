#include "Parser.h"

#include <QDebug>

static const quint8 mask_high1 = 0b10000000;
static const quint8 mask_low7 = 0b01111111;
static const quint8 mask_low3 = 0b00000111;

using namespace QNrbf;

bool Parser::readLengthPrefix(quint32 &out, QDataStream &in) {
    out = 0;

    quint32 size = 0;

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

    out = size;

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

    return true;
}

bool Parser::readUtf8Char(QChar &out, QDataStream &in) {
    quint8 head;
    in >> head;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    int size = (head & 0xD0) != 0 ? 4 : (head & 0xC0) != 0 ? 3 : (head & 0x80) != 0 ? 2 : 1;
    char buf[4];
    if (in.readRawData(buf, size) < 0) {
        return false;
    }
    quint16 value = (head & 0xD0) != 0 ? ((head & 0x3F) << 18) + ((buf[0] & 0x3F) << 12) +
                                             ((buf[1] & 0x3F) << 6) + (buf[0] & 0x3F)
                    : (head & 0xC0) != 0
                        ? ((head & 0x3F) << 12) + ((buf[0] & 0x3F) << 6) + (buf[1] & 0x3F)
                    : (head & 0x80) != 0 ? ((head & 0x3F) << 6) + (buf[0] & 0x3F)
                                         : (head & 0x3F);
    out = QChar(value);
    return true;
}
bool Parser::readDateTime(DateTime &out, QDataStream &in) {
    quint64 data;
    in >> data;
    if (in.status() != QDataStream::Ok)
        return false;
    out = DateTime(data);
    return true;
}

bool Parser::readDecimal(Decimal &out, QDataStream &in) {
    return readString(out._data, in);
}

bool Parser::readTimeSpan(TimeSpan &out, QDataStream &in) {
    quint64 data;
    in >> data;
    if (in.status() != QDataStream::Ok)
        return false;
    out = TimeSpan(data);
    return true;
}

bool Parser::writeLengthPrefix(quint32 size, QDataStream &out) {
    return true;
}

bool Parser::writeString(const QString &in, QDataStream &out) {
    if (!writeLengthPrefix(in.size(), out)) {
        return false;
    }
    QByteArray data = in.toUtf8();
    if (out.writeRawData(data.data(), data.size()) != data.size()) {
        return false;
    }
    return true;
}

bool Parser::writeUtf8Char(const QChar &in, QDataStream &out) {
    QByteArray data(QString(in).toUtf8());
    if (out.writeRawData(data.data(), data.size()) != data.size()) {
        return false;
    }
    return true;
}

bool Parser::writeDateTime(const QNrbf::DateTime &in, QDataStream &out) {
    out << in._data;
    return true;
}

bool Parser::writeDecimal(const QNrbf::Decimal &in, QDataStream &out) {
    writeString(in._data, out);
    return true;
}

bool Parser::writeTimeSpan(const QNrbf::TimeSpan &in, QDataStream &out) {
    out << in._data;
    return true;
}