#include "QNrbfStream.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>

#include "Formats/JsonReader.h"
#include "Formats/SvipReader.h"
#include "Formats/SvipWriter.h"

#include "Utils/NrbfParser.h"

#include "Utils/NrbfReader.h"
#include "Utils/NrbfWriter.h"

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

    NrbfWriter writer(q, reg);
    if (!writer.write()) {
        q->setStatus(QDataStream::WriteFailed);
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

QNrbfStream &QNrbfStream::operator>>(QChar &ch) {
    Parser::readUtf8Char(ch, *this);
    return *this;
}

QNrbfStream &QNrbfStream::operator>>(QString &str) {
    QString res;
    if (!Parser::readString(res, *this)) {
        str.clear();
    } else {
        str = std::move(res);
    }
    return *this;
}

QNrbfStream &QNrbfStream::operator>>(QJsonObject &obj) {
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

QNrbfStream &QNrbfStream::operator>>(XSAppModel &svip) {
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

QNrbfStream &QNrbfStream::operator<<(const QChar &ch) {
    Parser::writeUtf8Char(ch, *this);
    return *this;
}

QNrbfStream &QNrbfStream::operator<<(const QString &str) {
    Parser::writeString(str, *this);
    return *this;
}

QNrbfStream &QNrbfStream::operator<<(const XSAppModel &svip) {
    SvipWriter writer(svip);
    if (!writer.save()) {
        setStatus(QDataStream::ReadCorruptData);
    } else {
        d->serialize(writer.reg);
    }
    return *this;
}