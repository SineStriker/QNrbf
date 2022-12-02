#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>

#include <iostream>

#include "QNrbfStream.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        qDebug() << "No nrbf-file argument.";
        return 0;
    }

    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    QByteArray data;
    {
        QFile file(args.at(1));
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to load nrbf file";
            return -1;
        }

        data = file.readAll();
        file.close();
    }
    QNrbfStream in(&data, QIODevice::ReadOnly);

    QString name;
    QString ver;
    QJsonObject obj;

    // Read file head
    in >> name;
    in >> ver;

    // Read content
    in >> obj;

    if (in.status() != QDataStream::Ok) {
        qDebug() << "Failed to read nrbf record";
        return -1;
    }
    qDebug() << "Successfully load nrbf file";

    if (argc < 3) {
        qDebug() << "No output file argument.";
        return 0;
    }

    QJsonDocument doc;
    doc.setObject(obj);
    {
        QFile file(args.at(2));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            qDebug() << "Failed to create output file";
            return -1;
        }
        file.write(doc.toJson());
        file.close();
    }
    qDebug() << "Successfully output to json file";

    return 0;
}
