#include "QNrbfStream.h"

#include <QDebug>
#include <QJsonArray>

#include "Formats/JsonFormat.h"
#include "Formats/SvipFormat.h"
#include "Primitive/Parser.h"
#include "Utils/NrbfReader.h"

QNRBF_USING_NAMESPACE

class QNrbfStreamPrivate {
public:
    explicit QNrbfStreamPrivate(QNrbfStream *q) : q(q) {
    }

    void init();

    ObjectRef deserialize();

    QNrbfStream *q;
};

void QNrbfStreamPrivate::init() {
    Q_UNUSED(this);

    q->setByteOrder(QDataStream::LittleEndian);
}

ObjectRef QNrbfStreamPrivate::deserialize() {
    Q_UNUSED(this);

    NrbfReader reader(q);
    auto binObj = reader.read();
    if (reader.status() != NrbfReader::ReachEnd) {
        q->setStatus(QDataStream::ReadCorruptData);
    }
    return binObj;
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
    auto binObj = d->deserialize();
    if (status() == Ok) {
        JsonFormat fmt;
        if (!fmt.load(binObj)) {
            setStatus(QDataStream::ReadCorruptData);
        } else {
            obj = fmt.jsonObj;
        }
    }
    return *this;
}

QDataStream &QNrbfStream::operator>>(XSAppModel &svip) {
    auto binObj = d->deserialize();
    if (status() == Ok) {
        SvipFormat fmt;
        if (!fmt.load(binObj)) {
            setStatus(QDataStream::ReadCorruptData);
        } else {
            svip = fmt.appModel;
        }
    }
    return *this;
}
