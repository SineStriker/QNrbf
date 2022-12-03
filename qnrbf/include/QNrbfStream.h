#ifndef QNRBF_QNRBFSTREAM_H
#define QNRBF_QNRBFSTREAM_H

#include <QDataStream>
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

public:
    QDataStream &operator>>(QString &str);
    QDataStream &operator>>(QJsonObject &obj);
    QDataStream &operator>>(QNrbf::XSAppModel &svip);

protected:
    QScopedPointer<QNrbfStreamPrivate> d;
};

#endif // QNRBF_QNRBFSTREAM_H
