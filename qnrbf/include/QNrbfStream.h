#ifndef QNRBF_QNRBFSTREAM_H
#define QNRBF_QNRBFSTREAM_H

#include <QJsonObject>

#include "XStudio/XSAppModel.h"

class QNrbfStreamPrivate;

class QNRBF_API QNrbfStream : public QDataStream {
public:
    QNrbfStream();
    explicit QNrbfStream(QIODevice *dev);
    QNrbfStream(QByteArray *in, QIODevice::OpenMode flags);
    QNrbfStream(const QByteArray &in);
    ~QNrbfStream();

    static QString libraryVersion();

public:
    QNrbfStream &operator>>(QChar &ch);
    QNrbfStream &operator>>(QString &str);
    QNrbfStream &operator>>(QJsonObject &obj);
    QNrbfStream &operator>>(QNrbf::XSAppModel &svip);

    QNrbfStream &operator<<(const QChar &ch);
    QNrbfStream &operator<<(const QString &str);
    QNrbfStream &operator<<(const QNrbf::XSAppModel &svip);

protected:
    QScopedPointer<QNrbfStreamPrivate> d;
};

#endif // QNRBF_QNRBFSTREAM_H
