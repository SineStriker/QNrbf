#ifndef QNRBFFORMAT_JSONREADER_H
#define QNRBFFORMAT_JSONREADER_H

#include <QJsonObject>

#include "Utils/NrbfRegistry.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL JsonReader {
public:
    explicit JsonReader(const NrbfRegistry &reg);

    bool load();

    QJsonObject jsonObj;

protected:
    NrbfRegistry reg;

    QSet<qint32> cachedIds;
    QSet<AbstractObject *> cachedObjs;

    QJsonValue dfs(const ObjectRef &binObj);
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_JSONREADER_H
