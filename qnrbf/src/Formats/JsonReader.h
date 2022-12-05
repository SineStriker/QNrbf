#ifndef QNRBFFORMAT_JSONREADER_H
#define QNRBFFORMAT_JSONREADER_H

#include <QJsonObject>

#include "Utils/NrbfRegistry.h"

QNRBF_BEGIN_NAMESPACE

class QNRBF_INTERNAL JsonReader {
public:
    explicit JsonReader(const NrbfRegistry &reg);
    ~JsonReader();

    bool load();

    QJsonObject jsonObj;

protected:
    NrbfRegistry reg;

    // Deprecated
    QSet<AbstractObject *> cachedObjs;
    QJsonValue dfs(const ObjectRef &binObj);

    // Use this instead
    QJsonValue dfs_nonRecursive(const ObjectRef &rootObj);

    QJsonValue dfs_shallow();
};

QNRBF_END_NAMESPACE


#endif // QNRBFFORMAT_JSONREADER_H
