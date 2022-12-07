#include "QNrbfStream.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>

#include "Formats/JsonReader.h"
#include "Formats/SvipReader.h"
#include "Formats/SvipWriter.h"
#include "Primitive/Parser.h"
#include "Utils/NrbfReader.h"

QNRBF_USING_NAMESPACE

class QNrbfStreamPrivate {
public:
    explicit QNrbfStreamPrivate(QNrbfStream *q) : q(q) {
    }

    void init();

    NrbfRegistry deserialize();
    void serialize(const NrbfRegistry &reg);

    QNrbfStream *q;
};

void QNrbfStreamPrivate::init() {
    Q_UNUSED(this);

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    q->setByteOrder(QDataStream::LittleEndian);
#else
    q->setByteOrder(QDataStream::BigEndian);
#endif
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

void QNrbfStreamPrivate::serialize(const NrbfRegistry &reg) {
    Q_UNUSED(this);

    JsonReader reader(reg);
    if (!reader.load()) {
        q->setStatus(QDataStream::ReadCorruptData);
    } else {
        QJsonDocument doc(reader.jsonObj);
        auto data = doc.toJson();
        q->writeRawData(data.data(), data.size());
    }
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

QDataStream &QNrbfStream::operator<<(const QString &str) {
    Parser::writeString(str, *this);
    return *this;
}

QDataStream &QNrbfStream::operator<<(const XSAppModel &svip) {
    SvipWriter writer(svip);
    if (!writer.save()) {
        setStatus(QDataStream::ReadCorruptData);
    } else {
        d->serialize(writer.reg);
    }
    return *this;
}