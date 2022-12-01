#include "QNrbfStream.h"

#include <QDebug>

#include "Primitive/Parser.h"

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

QDataStream &QNrbfStream::operator>>(QNrbfObject &cls) {
    ReadHelper reader(this);

    bool over = false;
    bool failed = false;
    while (!over && !failed) {
        reader.read();

        switch (reader.status()) {
            case ReadHelper::ReachEnd: {
                ObjectRef obj;
                if (!reader.finish(&obj)) {
                    failed = true;
                } else {
                    over = true;
                }
                break;
            }

            case ReadHelper::Normal:
                break;

            default: {
                failed = true;
                break;
            }
        }
    }

    if (failed) {
        if (status() == QDataStream::Ok) {
            setStatus(QDataStream::ReadCorruptData);
        }
    }

    return *this;
}
