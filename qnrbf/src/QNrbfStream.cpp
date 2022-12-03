#include "QNrbfStream.h"

#include <QDebug>
#include <QJsonArray>

#include "Formats/JsonReader.h"
#include "Formats/SvipReader.h"
#include "Primitive/Parser.h"
#include "Utils/NrbfReader.h"

QNRBF_USING_NAMESPACE

class QNrbfStreamPrivate {
public:
    explicit QNrbfStreamPrivate(QNrbfStream *q) : q(q) {
    }

    void init();

    NrbfRegistry deserialize();

    QNrbfStream *q;
};

void QNrbfStreamPrivate::init() {
    Q_UNUSED(this);

    q->setByteOrder(QDataStream::LittleEndian);
}

NrbfRegistry QNrbfStreamPrivate::deserialize() {
    Q_UNUSED(this);

    NrbfReader reader(q);
    auto reg = reader.read();
    if (reader.status() != NrbfReader::ReachEnd) {
        q->setStatus(QDataStream::ReadCorruptData);
    }
    return reg;
}

// ------------------------------------------------------------------------------------------------

QNrbfStream::QNrbfStream() : d(new QNrbfStreamPrivate(this)) {
    d->init();
}

QNrbfStream::QNrbfStream(QIODevice *dev) : QDataStream(dev), d(new QNrbfStreamPrivate(this)) {
    d->init();
}

QNrbfStream::QNrbfStream(QByteArray *in, QIODevice::OpenMode flags)
    : QDataStream(in, flags), d(new QNrbfStreamPrivate(this)) {
    d->init();
}

QNrbfStream::QNrbfStream(const QByteArray &in) : QDataStream(in), d(new QNrbfStreamPrivate(this)) {
    d->init();
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

QDataStream &QNrbfStream::operator>>(QJsonObject &obj) {
    auto reg = d->deserialize();
    if (status() == Ok) {
        JsonReader reader(reg);
        if (!reader.load()) {
            setStatus(QDataStream::ReadCorruptData);
        } else {
            obj = reader.jsonObj;
        }
    }
    return *this;
}

QDataStream &QNrbfStream::operator>>(XSAppModel &svip) {
    auto reg = d->deserialize();
    if (status() == Ok) {
        SvipReader reader(reg);
        if (!reader.load()) {
            setStatus(QDataStream::ReadCorruptData);
        } else {
            svip = reader.appModel;
        }
    }
    return *this;
}
