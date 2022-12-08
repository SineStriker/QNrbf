#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>

#include "QNrbfStream.h"
#include "Svip/BinSvipResolver.h"

int main(int argc, char *argv[]) {
    QStringList args;
    args.reserve(argc);
#ifdef Q_OS_WINDOWS
    for (int i = 0; i < argc; ++i) {
        args.append(QString::fromLocal8Bit(argv[i]));
    }
#else
    for (int i = 0; i < argc; ++i) {
        args.append(QString::fromUtf8(argv[i]));
    }
#endif

    if (argc < 2) {
        qDebug().noquote() << QString("Usage: %1 <svip path> [output path]")
                                  .arg(QFileInfo(args.front()).fileName());
        return 0;
    }

    QByteArray data;
    QString filename = args.at(1);
    {
        QFile file(filename);
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

    if (in.status() != QDataStream::Ok || name != "SVIP") {
        qDebug() << "Failed to read version.";
        return -1;
    }

    // Read content
    in >> svip;
    if (in.status() != QDataStream::Ok) {
        qDebug() << "Failed to parse nrbf content.";
        return -1;
    }
    qDebug() << "Successfully load nrbf file.";

    // Convert to OpenSVIP model
    QSvipModel model;
    BinSvipResolver resolver;
    if (!resolver.load(svip, model, {{"version", name + ver}})) {
        qDebug() << "Failed to convert to OpenSVIP model.";
        return -1;
    }

    // Save model
    {
        QString outputFilename;
        if (args.size() >= 3) {
            outputFilename = args.at(2);
        } else {
            outputFilename = QFileInfo(filename).baseName() + "_svip.json";
        }
        QFile file(outputFilename);
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
