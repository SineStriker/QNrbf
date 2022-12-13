#include <QChar>
#include <QString>

#include "QNrbfStream.h"

int main(int argc, char *argv[]) {
    qDebug() << QNrbfStream::libraryVersion();

    for (int i = 0x0; i < 0x7F; ++i) {
        QChar ch(i);
        QByteArray bytes = QString(ch).toUtf8();
        QNrbfStream in(&bytes, QIODevice::ReadOnly);
        in >> ch;
        if (in.status() != QDataStream::Ok) {
            qDebug() << "Read char fail at" << i;
            return -1;
        }
        qDebug() << i << bytes.size() << QString(ch);
    }

    for (int i = 0x4E00; i < 0x4E20; ++i) {
        QChar ch(i);
        QByteArray bytes = QString(ch).toUtf8();
        QNrbfStream in(&bytes, QIODevice::ReadOnly);
        in >> ch;
        if (in.status() != QDataStream::Ok) {
            qDebug() << "Read char fail at" << i;
            return -1;
        }
        qDebug() << i << bytes.size() << QString(ch);
    }

    return 0;
}