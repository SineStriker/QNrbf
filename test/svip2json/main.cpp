#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>

#include "QNrbfStream.h"
#include "SvipUtils.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    if (argc < 2) {
        qDebug().noquote() << QString("Usage: %1 <svip path> <output path>")
                                  .arg(QFileInfo(args.front()).fileName());
        qDebug() << "No svip file argument.";
        return 0;
    }
    if (argc < 3) {
        qDebug() << "No output path argument.";
        return 0;
    }

    QByteArray data;
    {
        QFile file(args.at(1));
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to load nrbf file.";
            return -1;
        }

        data = file.readAll();
        file.close();
    }

    QNrbfStream in(&data, QIODevice::ReadOnly);

    QString name;
    QString ver;
    QNrbf::XSAppModel svip;

    // Read name and version
    in >> name;
    in >> ver;

    if (in.status() != QDataStream::Ok) {
        qDebug() << "Failed to read version information.";
        return -1;
    }

    // Read content
    in >> svip;
    if (in.status() != QDataStream::Ok) {
        qDebug() << "Failed to load nrbf file.";
        return -1;
    }
    qDebug() << "Successfully load nrbf file.";

    // Convert to OpenSVIP model
    QSvipModel model;
    if (!SvipUtils::bin2Json(svip, name + ver, model)) {
        qDebug() << "Failed to convert to OpenSVIP model.";
        return -1;
    }

    // Save model
    {
        QFile file(args.at(2));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            qDebug() << "Failed to create output file.";
            return -1;
        }
        file.write(QJsonDocument(model.toJsonObject()).toJson());
        file.close();
    }

    qDebug() << "Successfully save OpenSVIP json file.";

    return 0;
}
