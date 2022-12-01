#ifndef QNRBFFORMAT_QNRBFOBJECT_H
#define QNRBFFORMAT_QNRBFOBJECT_H

#include <QMap>

#include "QNrbfValue.h"

class QNRBF_API QNrbfObject {
public:
    QString libraryId; // Leave empty if it's system class

    QMap<QString, QNrbfValue> members;
};

#endif // QNRBFFORMAT_QNRBFOBJECT_H
