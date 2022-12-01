#include <QCoreApplication>
#include <QDebug>
#include <QFile>

#include <iostream>

#include "QNrbfStream.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        qDebug() << "No nrbf-file argument.";
        return 0;
    }

    QCoreApplication a(argc, argv);

    QString filename = QString::fromLocal8Bit(argv[1]);
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to load nrbf file";
        return -1;
    }

    QByteArray data = file.readAll();
    file.close();

    QNrbfStream in(&data, QIODevice::ReadOnly);

    QString name;
    QString ver;
    QNrbfObject obj;

    // Read file head
    in >> name;
    in >> ver;

    // Read content
    in >> obj;

    fflush(stdout);
    fflush(stderr);

    if (in.status() != QDataStream::Ok) {
        qDebug() << "Failed to read nrbf record";
        return -1;
    }
    qDebug() << "Successfully load nrbf file";

    return 0;
}
