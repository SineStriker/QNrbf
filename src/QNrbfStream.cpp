#include "qnrbfstream.h"

#include <QDebug>

#include "Primitive/LengthPrefixedString.h"

#include "Utils/ReadHelper.h"

QNRBF_USING_NAMESPACE

static void init(QNrbfStream *stream) {
    stream->setByteOrder(QDataStream::LittleEndian);
}

QNrbfStream::QNrbfStream() {
    init(this);
}

QNrbfStream::QNrbfStream(QIODevice *dev) : QDataStream(dev) {
    init(this);
}

QNrbfStream::QNrbfStream(QByteArray *in, QIODevice::OpenMode flags) : QDataStream(in, flags) {
    init(this);
}

QNrbfStream::QNrbfStream(const QByteArray &in) : QDataStream(in) {
    init(this);
}

QNrbfStream::~QNrbfStream() {
}

QDataStream &QNrbfStream::operator>>(QString &str) {
    QString res;
    if (!Parser::readString(res, *this)) {
        str.clear();
    } else {
        str = std::move(res);
    }
    return *this;
}

QDataStream &QNrbfStream::operator>>(QNrbfObject &obj) {
    ReadHelper reader(this);

    bool over = false;
    while (!over) {
        reader.read();

        switch (reader.status()) {
            case ReadHelper::ReachEnd:
                over = true;
                break;
            case ReadHelper::Normal:
                break;
            default:
                if (status() == QDataStream::Ok) {
                    setStatus(QDataStream::ReadCorruptData);
                }
                over = true;
                break;
        }
    }

    return *this;
}
