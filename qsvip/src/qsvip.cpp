#include "qsvip.h"

#include <QByteArray>
#include <QJsonDocument>

#include "QNrbfStream.h"
#include "QSvipModel.h"
#include "SvipUtils.h"

struct QSvipData {
    QByteArray input;
    QByteArray output;
    QByteArray error;
    QtMessageHandler msgHandler;
};

static QSvipData *data_ptr = nullptr;

static void customMessageHandler(QtMsgType type, const QMessageLogContext &context,
                                 const QString &msg) {
    data_ptr->error.append(msg.toUtf8());
}

void qsvip_reader_init(char *buf, int size) {
    data_ptr = new QSvipData();
    data_ptr->input.append(buf, size);

    // Redirect qDebug to error message
    data_ptr->msgHandler = qInstallMessageHandler(customMessageHandler);
}

void qsvip_reader_free() {
    // Restore qDebug
    qInstallMessageHandler(data_ptr->msgHandler);

    delete data_ptr;
}

bool qsvip_reader_load() {
    QNrbfStream in(&data_ptr->input, QIODevice::ReadOnly);

    QString name;
    QString ver;
    QNrbf::XSAppModel svip;

    // Read name and version
    in >> name;
    in >> ver;

    if (in.status() != QDataStream::Ok || name != "SVIP") {
        qDebug() << "Failed to read version";
        return false;
    }

    // Read content
    in >> svip;
    if (in.status() != QDataStream::Ok) {
        return false;
    }

    // Convert to OpenSVIP model
    QSvipModel model;
    if (!SvipUtils::bin2Json(svip, name + ver, model)) {
        qDebug() << "Failed to convert to OpenSVIP model";
        return false;
    }

    data_ptr->output = QJsonDocument(model.toJsonObject()).toJson();
    return true;
}

int qsvip_reader_alloc_output() {
    return data_ptr->output.size();
}

int qsvip_reader_alloc_error() {
    return data_ptr->error.size();
}

char *qsvip_reader_get_output() {
    return data_ptr->output.data();
}

char *qsvip_reader_get_error() {
    return data_ptr->error.data();
}