#include "NrbfParser.h"

#include <QDebug>

static const quint8 mask_high1 = 0b10000000;
static const quint8 mask_high2 = 0b11000000;
static const quint8 mask_high3 = 0b11100000;
static const quint8 mask_high4 = 0b11110000;
static const quint8 mask_high5 = 0b11111000;

static const quint8 mask_low7 = 0b01111111;
static const quint8 mask_low3 = 0b00000111;

static const quint32 max_length = 2147483647;

using namespace QNrbf;

bool Parser::readLengthPrefix(quint32 &out, QDataStream &in) {
    quint32 &size = out;
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
    if (byte & mask_high5) {
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
        in.setStatus(QDataStream::ReadCorruptData);
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
    quint8 buf[4];
    auto &head = buf[0];

    // Read first byte
    in >> head;
    if (in.status() != QDataStream::Ok) {
        return false;
    }

    // Read suffix bytes
    int suffix = (head & mask_high5) == mask_high4 ? 3 : (
                 (head & mask_high4) == mask_high3 ? 2 : (
                 (head & mask_high3) == mask_high2 ? 1 : 0));
    if (in.readRawData((char *) buf + 1, suffix) != suffix) {
        return false;
    }

    QString str = QString::fromUtf8((char *) buf, suffix + 1);
    if (str.isEmpty()) {
        return false;
    }
    out = str.front();
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
    if (size > max_length) {
        return false;
    }

    int i = 0;
    while (i == 0 || (size > 0 && i <= 4)) {
        quint8 byte = size & mask_low7;
        size >>= 7;
        if (size > 0) {
            byte |= mask_high1;
        }
        out << byte;
        if (out.status() != QDataStream::Ok) {
            return false;
        }
        i++;
    }
    if (size > 0) {
        out << quint32(size & mask_low3);
        if (out.status() != QDataStream::Ok) {
            return false;
        }
    }
    return true;
}

bool Parser::writeString(const QString &in, QDataStream &out) {
    QByteArray data = in.toUtf8();
    if (!writeLengthPrefix(data.size(), out)) {
        return false;
    }
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
    out << in.ticks;
    return true;
}