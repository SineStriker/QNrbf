#ifndef QNRBF_QNRBFSTREAM_H
#define QNRBF_QNRBFSTREAM_H

#include <QDataStream>

#include <qnrbfobject.h>

class QNRBF_API QNrbfStream : public QDataStream {
public:
    QNrbfStream();
    explicit QNrbfStream(QIODevice *dev);
    QNrbfStream(QByteArray *in, QIODevice::OpenMode flags);
    QNrbfStream(const QByteArray &in);
    ~QNrbfStream();

public:
    QDataStream &operator>>(QString &str);
    QDataStream &operator>>(QNrbfObject &obj);
};

#endif // QNRBF_QNRBFSTREAM_H